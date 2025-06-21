#include <iostream>
#include <memory>
#include <QApplication> // Necesario para QApplication
#include <QThread>      // Necesario para crear hilos
#include "RemoteDiskClient.h"
#include "ControllerNode.h"
#include "HttpServer.h"
#include "MainWindow.h"  // Asegúrate de incluir la cabecera para MainWindow

class ServerThread : public QThread {
    Q_OBJECT

    void run() override {
        HttpServer server(controller, "localhost", 8080);
        server.start();
    }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);  // Iniciar QApplication

    std::cout << "INICIO DEL MAIN" << std::endl;

    // Crear RemoteDiskClients
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

    // Crear ControllerNode
    std::cout << "Creando ControllerNode..." << std::endl;
    std::shared_ptr<ControllerNode> controller;
    try {
        controller = std::make_shared<ControllerNode>("data/metadata/metadata.txt", disks);
    } catch (const std::exception& e) {
        std::cerr << "Error al crear ControllerNode: " << e.what() << std::endl;
    }

    // Ejecutar servidor en un hilo separado
    std::cout << "Iniciando servidor HTTP..." << std::endl;
    ServerThread *serverThread = new ServerThread();
    serverThread->start();

    // Crear la ventana principal de la interfaz gráfica
    MainWindow w;
    w.show();  // Mostrar la ventana principal

    std::cout << "FIN DEL MAIN" << std::endl;
    return a.exec();  // Ejecutar el bucle de eventos de Qt
}
