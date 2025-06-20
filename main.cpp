#include <iostream>
#include <memory>
#include "RemoteDiskClient.h"
#include "ControllerNode.h"
#include "HttpServer.h"

int main() {
    // Crear los clientes HTTP a los nodos remotos
    std::vector<std::shared_ptr<RemoteDiskClient>> disks;
    disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8081));
    disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8082));
    disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8083));
    disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8084));

    // Crear ControllerNode con archivo de metadatos
    auto controller = std::make_shared<ControllerNode>("data/metadata/metadata.txt", disks);

    // Iniciar servidor HTTP del controller
    HttpServer server(controller, "localhost", 8080);
    std::cout << "Servidor HTTP iniciado en http://localhost:8080" << std::endl;
    server.start();

    return 0;
}
