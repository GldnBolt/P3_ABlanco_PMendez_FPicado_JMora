#include "ControllerNode.h"
#include <stdexcept>
<<<<<<< HEAD
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

ControllerNode::ControllerNode(const std::string& metadataFile, const std::vector<std::shared_ptr<DiskNode>>& disks)
    : disks_(disks), numDisks_(disks.size()), metadataFile_(metadataFile)
{
    if (numDisks_ < 3)
        throw std::runtime_error("RAID5 requiere al menos 3 discos");
    // Crear directorio para metadatos
    fs::create_directories(fs::path(metadataFile_).parent_path());
    loadMetadata();
=======

ControllerNode::ControllerNode(const std::vector<std::shared_ptr<DiskNode>>& disks)
  : disks_(disks), numDisks_(disks.size())
{
    if (numDisks_ < 3)
        throw std::runtime_error("RAID5 requiere al menos 3 discos");
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e
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
<<<<<<< HEAD
=======
    // El disco de paridad rota: p = (numDisks_ - 1) - (stripeIndex % numDisks_)
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e
    int p = (numDisks_ - 1) - (stripeIndex % numDisks_);
    std::vector<std::string> dataBlocks;
    dataBlocks.reserve(numDisks_ - 1);

<<<<<<< HEAD
=======
    // Primero escribimos datos en todos menos en el de paridad
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e
    for (int d = 0; d < numDisks_; ++d) {
        if (d == p) continue;
        disks_[d]->writeBlock(stripeIndex, data);
        dataBlocks.push_back(data);
    }
<<<<<<< HEAD
=======
    // Luego calculamos y grabamos la paridad
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e
    std::string parity = xorParity(dataBlocks);
    disks_[p]->writeBlock(stripeIndex, parity);
}

std::string ControllerNode::readStripe(int stripeIndex) {
    int p = (numDisks_ - 1) - (stripeIndex % numDisks_);
    std::vector<std::string> dataBlocks(numDisks_);
    int missing = -1;

<<<<<<< HEAD
    for (int d = 0; d < numDisks_; ++d) {
        try {
            dataBlocks[d] = disks_[d]->readBlock(stripeIndex);
        }
        catch (...) {
=======
    // Intentamos leer todos; detectamos si alguno falla (excepción)
    for (int d = 0; d < numDisks_; ++d) {
        try {
            dataBlocks[d] = disks_[d]->readBlock(stripeIndex);
        } catch (...) {
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e
            missing = d;
        }
    }

<<<<<<< HEAD
=======
    // Si todo OK, devolvemos el primero de datos (ejemplo)
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e
    if (missing == -1) {
        for (int d = 0; d < numDisks_; ++d) {
            if (d != p) return dataBlocks[d];
        }
    }

<<<<<<< HEAD
=======
    // Si hubo un fallo, reconstruimos:
    // block_missing = xorParity( todos excepto missing )
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e
    std::vector<std::string> others;
    others.reserve(numDisks_ - 1);
    for (int d = 0; d < numDisks_; ++d) {
        if (d == missing) continue;
        others.push_back(dataBlocks[d]);
    }
<<<<<<< HEAD
    return xorParity(others);
}

void ControllerNode::loadMetadata() {
    std::ifstream ifs(metadataFile_);
    if (!ifs.is_open()) return;
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

void ControllerNode::addDocument(const std::string& docName, const std::string& content) {
    if (docToStripes_.find(docName) != docToStripes_.end()) {
        deleteDocument(docName); // Sobrescribe eliminando el anterior
    }
    int blockSize = disks_[0]->getBlockSize(); // Usar blockSize del primer disco
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
}

void ControllerNode::deleteDocument(const std::string& docName) {
    auto it = docToStripes_.find(docName);
    if (it == docToStripes_.end()) {
        throw std::runtime_error("Documento no encontrado: " + docName);
    }
    for (int stripeIndex : it->second) {
        for (int d = 0; d < numDisks_; ++d) {
            try {
                disks_[d]->writeBlock(stripeIndex, std::string(disks_[0]->getBlockSize(), '\0'));
            }
            catch (...) {
            }
        }
    }
    docToStripes_.erase(it);
    saveMetadata();
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
    if (docName.ends_with(".txt")) mime = "text/plain";
    else if (docName.ends_with(".pdf")) mime = "application/pdf";
    else if (docName.ends_with(".jpg") || docName.ends_with(".jpeg")) mime = "image/jpeg";
    // Nota: El servidor HTTP usará este mime al devolverlo
    return content;
}
=======
    std::string recovered = xorParity(others);
    return recovered;
}
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e
