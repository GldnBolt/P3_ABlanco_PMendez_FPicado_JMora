#include <iostream>
#include <memory>
#include "DiskNode.h"
#include "ControllerNode.h"
#include "HttpServer.h"

int main() {
    // Inicializa 4 nodos con sus configs
    std::vector<std::shared_ptr<DiskNode>> disks;
    for (int i = 1; i <= 4; ++i) {
        disks.push_back(std::make_shared<DiskNode>("config/node" + std::to_string(i) + ".xml"));
    }

    // Crear ControllerNode con archivo de metadatos configurable
    auto controller = std::make_shared<ControllerNode>("data/metadata/metadata.txt", disks);

    // Iniciar servidor HTTP
    HttpServer server(controller, "localhost", 8080);
    std::cout << "Servidor HTTP iniciado en http://localhost:8080" << std::endl;
    server.start();

    return 0;
}