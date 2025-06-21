//
// Created by Frand on 19/06/2025.
//
#include "DiskNode.h"
#include "extern/httplib.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string config = argc > 1 ? argv[1] : "config/config.xml";
    DiskNode node(config);

    httplib::Server server;

    server.Post("/writeBlock", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            int index = std::stoi(req.get_param_value("index"));
            node.writeBlock(index, req.body);
            res.set_content("OK", "text/plain");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(e.what(), "text/plain");
        }
    });

    server.Get("/readBlock", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            int index = std::stoi(req.get_param_value("index"));
            std::string data = node.readBlock(index);
            res.set_content(data, "application/octet-stream");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(e.what(), "text/plain");
        }
    });

    std::cout << "DiskNode listo en http://localhost:8080" << std::endl;
    server.listen("localhost", 8080);
    return 0;
}

