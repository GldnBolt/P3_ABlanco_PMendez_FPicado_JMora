#include "ServerThread.h"  // Incluir el archivo de encabezado
#include <HttpServer.h>

ServerThread::ServerThread(std::shared_ptr<ControllerNode> ctrl) : controller(ctrl) {}

void ServerThread::run() {
    HttpServer server(controller, "localhost", 8080);
    server.start();
}
