//
// Created by Frand on 19/06/2025.
//

#include "RemoteDiskClient.h"
#include "extern/httplib.h"
#include <stdexcept>

RemoteDiskClient::RemoteDiskClient(const std::string& ip, int port)
    : ip_(ip), port_(port) {}

std::string RemoteDiskClient::readBlock(int index) {
    httplib::Client cli(ip_, port_);
    std::string path = "/readBlock?index=" + std::to_string(index);
    auto res = cli.Get(path.c_str());
    if (!res || res->status != 200) {
        throw std::runtime_error("Error leyendo bloque del nodo: " + ip_);
    }
    return res->body;
}
int RemoteDiskClient::getBlockSize() const {
    return 1024;
}

void RemoteDiskClient::writeBlock(int index, const std::string& data) {
    httplib::Client cli(ip_, port_);
    std::string path = "/writeBlock?index=" + std::to_string(index);
    auto res = cli.Post(path.c_str(), data, "application/octet-stream");
    if (!res || res->status != 200) {
        throw std::runtime_error("Error escribiendo bloque en nodo: " + ip_);
    }


}

