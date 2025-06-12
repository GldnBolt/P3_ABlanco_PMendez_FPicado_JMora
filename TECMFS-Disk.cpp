#include "TECMFS-Disk.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <msxml6.h>
#include <comdef.h>

DiskNode::DiskNode(std::string ip, int port, std::string storagePath)
    : ip_(ip), port_(port), storagePath_(storagePath) {
    // Cargar configuración de bloques desde el archivo XML
    loadConfiguration(storagePath);
}

DiskNode::~DiskNode() {
    // Aquí puedes liberar recursos si es necesario
}

// Leer un bloque de datos
std::string DiskNode::readBlock(int blockIndex) {
    if (blockIndex < 0 || blockIndex >= blocks_.size()) {
        throw std::out_of_range("Block index out of range");
    }
    return blocks_[blockIndex];
}

// Escribir datos en un bloque
void DiskNode::writeBlock(int blockIndex, const std::string& data) {
    if (blockIndex < 0 || blockIndex >= blocks_.size()) {
        throw std::out_of_range("Block index out of range");
    }
    blocks_[blockIndex] = data;
}

// Obtener información del nodo
std::string DiskNode::getInfo() const {
    std::stringstream info;
    info << "IP: " << ip_ << ", Port: " << port_ << ", Storage: " << storagePath_;
    return info.str();
}

// Cargar configuración de bloques desde el archivo XML usando MSXML
void DiskNode::loadConfiguration(const std::string& configPath) {
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        std::cerr << "Error inicializando COM" << std::endl;
        return;
    }

    // Crear un objeto DOMDocument para cargar el XML
    IXMLDOMDocument6* pXMLDoc = NULL;
    hr = CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pXMLDoc));
    if (FAILED(hr) || pXMLDoc == NULL) {
        std::cerr << "Error creando DOMDocument" << std::endl;
        CoUninitialize();
        return;
    }

    // Cargar el archivo XML
    VARIANT_BOOL isSuccess;
    hr = pXMLDoc->load(_variant_t(configPath.c_str()), &isSuccess);
    if (FAILED(hr) || isSuccess == VARIANT_FALSE) {
        std::cerr << "Error cargando el archivo XML" << std::endl;
        pXMLDoc->Release();
        CoUninitialize();
        return;
    }

    // Leer los elementos del XML
    IXMLDOMNodeList* pNodeList = NULL;
    hr = pXMLDoc->getElementsByTagName(_bstr_t(L"Block"), &pNodeList);
    if (FAILED(hr)) {
        std::cerr << "Error obteniendo los elementos 'Block'" << std::endl;
        pXMLDoc->Release();
        CoUninitialize();
        return;
    }

    // Iterar a través de los nodos "Block" y almacenarlos en la lista de bloques
    IXMLDOMNode* pNode = NULL;
    hr = pNodeList->nextNode(&pNode);
    while (hr == S_OK && pNode != NULL) {
        // Obtener el texto del nodo
        BSTR bstrText = NULL;
        pNode->get_text(&bstrText);
        blocks_.push_back(_bstr_t(bstrText));  // Añadir el bloque a la lista
        SysFreeString(bstrText);  // Liberar el texto extraído del nodo

        // Obtener el siguiente nodo
        hr = pNodeList->nextNode(&pNode);
    }

    // Liberar recursos de MSXML
    pNodeList->Release();
    pXMLDoc->Release();
    CoUninitialize();
}
