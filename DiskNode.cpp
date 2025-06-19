<<<<<<< HEAD

#include "DiskNode.h"
#include "tinyxml2.h"           
#include <iostream>
#include <fstream>
#include <filesystem>           
=======
//
// Created by Xpc on 15/6/2025.
//
#include "DiskNode.h"
#include "tinyxml2.h"           // Asegúrate de que tu include path lo encuentra
#include <iostream>
#include <fstream>
#include <filesystem>           // C++17: para crear carpetas
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e

using namespace tinyxml2;
namespace fs = std::filesystem;

DiskNode::DiskNode(const std::string& configPath) {
    loadConfiguration(configPath);
    initializeStorage();
}

DiskNode::~DiskNode() {}

// Construye el nombre de archivo para el bloque i
std::string DiskNode::blockFilename(int i) const {
    return storageDir_ + "/block_" + std::to_string(i) + ".dat";
}

void DiskNode::loadConfiguration(const std::string& configPath) {
    XMLDocument doc;
    if (doc.LoadFile(configPath.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("No se pudo cargar config XML: " + configPath);
    }
    XMLElement* root = doc.FirstChildElement("NodeConfiguration");
<<<<<<< HEAD
    ip_ = root->FirstChildElement("IP")->GetText();
    port_ = std::stoi(root->FirstChildElement("Port")->GetText());
    storageDir_ = root->FirstChildElement("StorageDir")->GetText();
    numBlocks_ = std::stoi(root->FirstChildElement("NumBlocks")->GetText());
    blockSize_ = std::stoi(root->FirstChildElement("BlockSize")->GetText());
=======
    ip_         = root->FirstChildElement("IP")->GetText();
    port_       = std::stoi(root->FirstChildElement("Port")->GetText());
    storageDir_ = root->FirstChildElement("StorageDir")->GetText();
    numBlocks_  = std::stoi(root->FirstChildElement("NumBlocks")->GetText());
    blockSize_  = std::stoi(root->FirstChildElement("BlockSize")->GetText());
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e

    blocks_.resize(numBlocks_);  // reservado para cache si quieres
}

void DiskNode::initializeStorage() {
    // Crear carpeta si no existe
    if (!fs::exists(storageDir_)) {
        fs::create_directories(storageDir_);
    }
    // Para cada bloque: crear archivo con tamaño fijo (si no existe)
    for (int i = 0; i < numBlocks_; ++i) {
        std::string fn = blockFilename(i);
        if (!fs::exists(fn)) {
            // Creamos archivo de tamaño blockSize_ con ceros
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
    // Si quieres rellenar con ceros el resto:
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
<<<<<<< HEAD
}
=======
}
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e
