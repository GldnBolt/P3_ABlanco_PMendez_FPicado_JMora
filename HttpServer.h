#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "ControllerNode.h"
#include "httplib.h"
#include <memory>

class HttpServer {
public:
    HttpServer(std::shared_ptr<ControllerNode> controller, const std::string& host = "localhost", int port = 8080);
    void start();

private:
    std::shared_ptr<ControllerNode> controller_;
    httplib::Server svr_;
    std::string host_;
    int port_;

    void setupRoutes();
};

#endif // HTTPSERVER_H