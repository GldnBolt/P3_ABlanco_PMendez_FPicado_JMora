// main.cpp

#include <iostream>
#include <QApplication>
#include <thread>

#include "RemoteDiskClient.h"
#include "ControllerNode.h"
#include "HttpServer.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    // 0) Inicializa Qt
    QApplication app(argc, argv);

    std::cout << "INICIO DEL MAIN" << std::endl;

    // 1) Crear RemoteDiskClients apuntando a cada DiskNodeServer
    std::vector<std::shared_ptr<RemoteDiskClient>> disks;
    try {
        disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8081));
        disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8082));
        disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8083));
        disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8084));
        std::cout << "RemoteDiskClients creados." << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << "Error creando RemoteDiskClient: " << e.what() << std::endl;
        return -1;
    }

    // 2) Crear ControllerNode, indicando la ruta a metadata.txt
    std::shared_ptr<ControllerNode> controller;
    try {
        controller = std::make_shared<ControllerNode>(
            "data/metadata/metadata.txt",
            disks
        );
        std::cout << "ControllerNode creado." << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << "Error al crear ControllerNode: " << e.what() << std::endl;
        return -1;
    }

    // 3) Arrancar servidor HTTP en hilo separado
    std::thread serverThread([controller]() {
        try {
            HttpServer server(controller, "localhost", 8080);
            std::cout << "Servidor HTTP iniciado en http://localhost:8080" << std::endl;
            server.start();
        }
        catch (const std::exception &e) {
            std::cerr << "Error en HttpServer: " << e.what() << std::endl;
        }
    });
    serverThread.detach();  // No bloquea el hilo principal

    // 4) Crear y mostrar la ventana principal, pasándole el controller
    MainWindow w(controller);
    w.show();

    std::cout << "FIN DEL MAIN" << std::endl;
    return app.exec();
}
