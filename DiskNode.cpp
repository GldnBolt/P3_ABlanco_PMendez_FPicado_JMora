#include "DiskNode.h"
#include "tinyxml2.h"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace tinyxml2;
namespace fs = std::filesystem;

DiskNode::DiskNode(const std::string& configPath) {
    loadConfiguration(configPath);
    initializeStorage();
}

DiskNode::~DiskNode() {}

std::string DiskNode::blockFilename(int i) const {
    return storageDir_ + "/block_" + std::to_string(i) + ".dat";
}

void DiskNode::loadConfiguration(const std::string& configPath) {
    XMLDocument doc;
    if (doc.LoadFile(configPath.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("No se pudo cargar config XML: " + configPath);
    }
    XMLElement* root = doc.FirstChildElement("NodeConfiguration");
    ip_ = root->FirstChildElement("IP")->GetText();
    port_ = std::stoi(root->FirstChildElement("Port")->GetText());
    storageDir_ = root->FirstChildElement("StorageDir")->GetText();
    numBlocks_ = std::stoi(root->FirstChildElement("NumBlocks")->GetText());
    blockSize_ = std::stoi(root->FirstChildElement("BlockSize")->GetText());

    blocks_.resize(numBlocks_);
}

void DiskNode::initializeStorage() {
    if (!fs::exists(storageDir_)) {
        fs::create_directories(storageDir_);
    }
    for (int i = 0; i < numBlocks_; ++i) {
        std::string fn = blockFilename(i);
        if (!fs::exists(fn)) {
            std::ofstream ofs(fn, std::ios::binary);
            std::string zeros(blockSize_, '\0');
            ofs.write(zeros.data(), zeros.size());
        }
    }
}

std::string DiskNode::readBlock(int blockIndex) {
    if (blockIndex < 0 || blockIndex >= numBlocks_)
        throw std::out_of_range("Índice de bloque fuera de rango");
    std::string data;
    data.resize(blockSize_);
    std::ifstream ifs(blockFilename(blockIndex), std::ios::binary);
    ifs.read(&data[0], blockSize_);
    return data;
}

void DiskNode::writeBlock(int blockIndex, const std::string& data) {
    if (blockIndex < 0 || blockIndex >= numBlocks_)
        throw std::out_of_range("Índice de bloque fuera de rango");
    if (data.size() > (size_t)blockSize_)
        throw std::length_error("Data excede el tamaño de bloque");

    std::ofstream ofs(blockFilename(blockIndex), std::ios::binary);
    ofs.write(data.data(), data.size());
    if (data.size() < (size_t)blockSize_) {
        std::string pad(blockSize_ - data.size(), '\0');
        ofs.write(pad.data(), pad.size());
    }
}

std::string DiskNode::getInfo() const {
    std::ostringstream oss;
    oss << "IP=" << ip_
        << " Port=" << port_
        << " StorageDir=" << storageDir_
        << " NumBlocks=" << numBlocks_
        << " BlockSize=" << blockSize_;
    return oss.str();
}