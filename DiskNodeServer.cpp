//
// Created by Frand on 18/06/2025.
//

#include "extern/httplib.h"
#include "DiskNode.h"
#include <iostream>

int main() {
    try {
        // Carga configuración desde XML
        DiskNode node("config/config_node1.xml");

        // Servidor HTTPS: usa los certificados autofirmados
        httplib::SSLServer server("cert.pem", "key.pem");

        if (!server.is_valid()) {
            std::cerr << "Error al iniciar el servidor HTTPS. Revisa los certificados." << std::endl;
            return 1;
        }

        // Endpoint para escribir un bloque
        server.Post("/writeBlock", [&](const httplib::Request& req, httplib::Response& res) {
            try {
                int index = std::stoi(req.get_param_value("index"));
                const std::string& data = req.body;
                node.writeBlock(index, data);
                res.set_content("OK", "text/plain");
            } catch (const std::exception& e) {
                res.status = 500;
                res.set_content(std::string("Error: ") + e.what(), "text/plain");
            }
        });

        // Endpoint para leer un bloque
        server.Get("/readBlock", [&](const httplib::Request& req, httplib::Response& res) {
            try {
                int index = std::stoi(req.get_param_value("index"));
                std::string data = node.readBlock(index);
                res.set_content(data, "application/octet-stream");
            } catch (const std::exception& e) {
                res.status = 500;
                res.set_content(std::string("Error: ") + e.what(), "text/plain");
            }
        });

        std::cout << "Servidor DiskNode escuchando en puerto 8080 (HTTPS)..." << std::endl;
        server.listen("0.0.0.0", 8080);

    } catch (const std::exception& e) {
        std::cerr << "Excepción al iniciar DiskNode: " << e.what() << std::endl;
        return 1;
    }
}

