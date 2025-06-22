#include "DiskNode.h"
#include "Extern/httplib.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <config.xml> <puerto>\n";
        return 1;
    }

    const std::string configFile = argv[1];
    int port = 0;
    try {
        port = std::stoi(argv[2]);
    } catch (...) {
        std::cerr << "Puerto inválido: " << argv[2] << "\n";
        return 1;
    }

    try {
        // Carga la configuración del disco (tamaño de bloque, ruta de datos, etc.)
        DiskNode node(configFile);

        // Monta el servidor HTTP sobre 0.0.0.0:port
        httplib::Server svr;

        // Endpoint para escritura: /writeBlock?index=<i>
        svr.Post("/writeBlock", [&node](const httplib::Request& req,
                                        httplib::Response& res) {
            auto idxIt = req.params.find("index");
            if (idxIt == req.params.end()) {
                res.status = 400;
                res.set_content("Falta parámetro index", "text/plain");
                return;
            }
            int index = std::stoi(idxIt->second);
            try {
                node.writeBlock(index, req.body);
                res.set_content("OK", "text/plain");
            } catch (const std::exception& e) {
                res.status = 500;
                res.set_content(std::string("Error: ") + e.what(), "text/plain");
            }
        });

        // Endpoint para lectura: /readBlock?index=<i>
        svr.Get("/readBlock", [&node](const httplib::Request& req,
                                       httplib::Response& res) {
            auto idxIt = req.params.find("index");
            if (idxIt == req.params.end()) {
                res.status = 400;
                res.set_content("Falta parámetro index", "text/plain");
                return;
            }
            int index = std::stoi(idxIt->second);
            try {
                auto block = node.readBlock(index);
                res.set_content(block, "application/octet-stream");
            } catch (const std::exception& e) {
                res.status = 500;
                res.set_content(std::string("Error: ") + e.what(), "text/plain");
            }
        });

        std::cout << "DiskNode listo en http://0.0.0.0:" << port
                  << " (config: " << configFile << ")\n";
        svr.listen("0.0.0.0", port);
    }
    catch (const std::exception& e) {
        std::cerr << "Error arrancando DiskNodeServer: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
