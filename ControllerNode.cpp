#include "ControllerNode.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

ControllerNode::ControllerNode(const std::string& metadataFile, const std::vector<std::shared_ptr<RemoteDiskClient>>& clients)
    : remoteDisks_(clients), numDisks_(clients.size()), metadataFile_(metadataFile)
{
    if (numDisks_ < 3)
        throw std::runtime_error("RAID5 requiere al menos 3 discos");

    // Verifica la existencia del directorio antes de crear
    if (!fs::exists(fs::path(metadataFile_).parent_path())) {
        fs::create_directories(fs::path(metadataFile_).parent_path());
    }

    loadMetadata(); // Asegúrate de que el archivo XML se esté cargando
}

std::string ControllerNode::xorParity(const std::vector<std::string>& blocks) {
    if (blocks.empty()) return "";
    size_t sz = blocks[0].size();
    std::string parity(sz, '\0');
    for (size_t i = 0; i < sz; ++i) {
        char p = 0;
        for (auto& blk : blocks) {
            p ^= blk[i];
        }
        parity[i] = p;
    }
    return parity;
}

void ControllerNode::writeStripe(int stripeIndex, const std::string& data) {
    int p = (numDisks_ - 1) - (stripeIndex % numDisks_);
    std::vector<std::string> dataBlocks;
    dataBlocks.reserve(numDisks_ - 1);

    for (int d = 0; d < numDisks_; ++d) {
        if (d == p) continue;
        remoteDisks_[d]->writeBlock(stripeIndex, data);
        dataBlocks.push_back(data);
    }
    std::string parity = xorParity(dataBlocks);
    remoteDisks_[p]->writeBlock(stripeIndex, parity);
}

std::string ControllerNode::readStripe(int stripeIndex) {
    int p = (numDisks_ - 1) - (stripeIndex % numDisks_);
    std::vector<std::string> dataBlocks(numDisks_);
    int missing = -1;

    for (int d = 0; d < numDisks_; ++d) {
        try {
            dataBlocks[d] = remoteDisks_[d]->readBlock(stripeIndex);
        }
        catch (...) {
            missing = d;
        }
    }

    if (missing == -1) {
        for (int d = 0; d < numDisks_; ++d) {
            if (d != p) return dataBlocks[d];
        }
    }

    std::vector<std::string> others;
    others.reserve(numDisks_ - 1);
    for (int d = 0; d < numDisks_; ++d) {
        if (d == missing) continue;
        others.push_back(dataBlocks[d]);
    }
    std::string recovered = xorParity(others);
    return recovered;
}

void ControllerNode::loadMetadata() {
    std::ifstream ifs(metadataFile_);
    if (!ifs.is_open()) {
        throw std::runtime_error("No se pudo cargar config XML: " + metadataFile_);
    }

    std::string line;
    while (std::getline(ifs, line)) {
        std::istringstream iss(line);
        std::string docName;
        int stripe;
        std::vector<int> stripes;
        iss >> docName;
        while (iss >> stripe) {
            stripes.push_back(stripe);
        }
        docToStripes_[docName] = stripes;
    }
}

void ControllerNode::saveMetadata() {
    std::ofstream ofs(metadataFile_);
    for (const auto& [docName, stripes] : docToStripes_) {
        ofs << docName;
        for (int stripe : stripes) {
            ofs << " " << stripe;
        }
        ofs << "\n";
    }
}

bool ControllerNode::addDocument(const std::string& docName, const std::string& content) {
    if (docToStripes_.find(docName) != docToStripes_.end()) {
        deleteDocument(docName);
    }
    int blockSize = remoteDisks_[0]->getBlockSize();
    std::vector<int> stripes;
    for (size_t i = 0; i < content.size(); i += blockSize) {
        std::string chunk = content.substr(i, blockSize);
        int stripeIndex = 0;
        while (true) {
            bool used = false;
            for (const auto& [_, s] : docToStripes_) {
                for (int stripe : s) {
                    if (stripe == stripeIndex) {
                        used = true;
                        break;
                    }
                }
            }
            if (!used) break;
            ++stripeIndex;
        }
        writeStripe(stripeIndex, chunk);
        stripes.push_back(stripeIndex);
    }
    docToStripes_[docName] = stripes;
    saveMetadata();
    return true;  // Se debe retornar true si el documento fue agregado correctamente
}

bool ControllerNode::deleteDocument(const std::string& docName) {
    auto it = docToStripes_.find(docName);
    if (it == docToStripes_.end()) {
        throw std::runtime_error("Documento no encontrado: " + docName);
    }
    for (int stripeIndex : it->second) {
        for (int d = 0; d < numDisks_; ++d) {
            try {
                remoteDisks_[d]->writeBlock(stripeIndex, std::string(remoteDisks_[0]->getBlockSize(), '\0'));
            }
            catch (...) {}
        }
    }
    docToStripes_.erase(it);
    saveMetadata();
    return true;
}

std::vector<std::string> ControllerNode::searchDocument(const std::string& docName, bool exact) {
    std::vector<std::string> results;
    for (const auto& [name, _] : docToStripes_) {
        std::string lowerName = name;
        std::string lowerDocName = docName;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        std::transform(lowerDocName.begin(), lowerDocName.end(), lowerDocName.begin(), ::tolower);
        if (exact ? (lowerName == lowerDocName) : (lowerName.find(lowerDocName) != std::string::npos)) {
            results.push_back(name);
        }
    }
    return results;
}

std::string ControllerNode::downloadDocument(const std::string& docName) {
    auto it = docToStripes_.find(docName);
    if (it == docToStripes_.end()) {
        throw std::runtime_error("Documento no encontrado: " + docName);
    }
    std::string content;
    for (int stripeIndex : it->second) {
        std::string chunk = readStripe(stripeIndex);
        size_t end = chunk.find('\0');
        chunk = (end != std::string::npos) ? chunk.substr(0, end) : chunk;
        content += chunk;
    }
    std::string mime = "application/octet-stream";
    if (docName.length() >= 4 && docName.substr(docName.length() - 4) == ".txt") mime = "text/plain";
    else if (docName.length() >= 4 && docName.substr(docName.length() - 4) == ".pdf") mime = "application/pdf";
    else if ((docName.length() >= 4 && docName.substr(docName.length() - 4) == ".jpg") ||
        (docName.length() >= 5 && docName.substr(docName.length() - 5) == ".jpeg")) mime = "image/jpeg";
    return content;
}

std::vector<std::string> ControllerNode::getDocuments() {
    std::vector<std::string> documentNames;
    for (const auto& pair : docToStripes_) {
        documentNames.push_back(pair.first);  // Agregar el nombre del documento
    }
    return documentNames;  // Devuelve los nombres de los documentos
}

