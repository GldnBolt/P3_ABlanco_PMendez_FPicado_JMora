#include <iostream>
#include <memory>
#include "RemoteDiskClient.h"
#include "ControllerNode.h"
#include "HttpServer.h"

int main() {
    std::cout << "INICIO DEL MAIN" << std::endl;

    std::vector<std::shared_ptr<RemoteDiskClient>> disks;

    std::cout << "Creando RemoteDiskClients..." << std::endl;
    try {
        disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8081));
        disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8082));
        disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8083));
        disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8084));
    } catch (const std::exception& e) {
        std::cerr << "Error creando RemoteDiskClient: " << e.what() << std::endl;
    }

    std::cout << "Creando ControllerNode..." << std::endl;
    std::shared_ptr<ControllerNode> controller;
    try {
        controller = std::make_shared<ControllerNode>("data/metadata/metadata.txt", disks);
    } catch (const std::exception& e) {
        std::cerr << "Error al crear ControllerNode: " << e.what() << std::endl;
    }

    std::cout << "Iniciando servidor HTTP..." << std::endl;
    try {
        HttpServer server(controller, "localhost", 8080);
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Error al iniciar el servidor: " << e.what() << std::endl;
    }

    std::cout << "FIN DEL MAIN" << std::endl;
    return 0;
}