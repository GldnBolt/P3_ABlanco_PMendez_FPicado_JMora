#include "HttpServer.h"
#include <sstream>

HttpServer::HttpServer(std::shared_ptr<ControllerNode> controller, const std::string& host, int port)
    : controller_(controller), host_(host), port_(port), svr_() {
    setupRoutes();
}

void HttpServer::setupRoutes() {
    // Endpoint para agregar un documento
    svr_.Post("/documents", [this](const httplib::Request& req, httplib::Response& res) {
        std::cout << "Recibida solicitud POST. Par�metros: ";
        for (const auto& param : req.params) {
            std::cout << param.first << "=" << param.second << ", ";
        }
        std::cout << std::endl;

        for (const auto& file : req.files) {
            std::cout << "Archivo/Par�metro: " << file.first << ", valor: " << file.second.content << std::endl;
        }

        if (req.has_file("file")) {
            auto file = req.get_file_value("file");
            std::cout << "Archivo recibido: " << file.filename << ", contenido: " << file.content.substr(0, 10) << "..." << std::endl;
        }
        else {
            std::cout << "No se encontr� archivo 'file'" << std::endl;
        }

        auto name = req.get_param_value("name");
        if (name.empty()) {
            // Intenta obtener name de req.files si no est� en params
            for (const auto& file : req.files) {
                if (file.first == "name") {
                    name = file.second.content;
                    break;
                }
            }
        }
        std::cout << "Par�metro name: " << (name.empty() ? "NO ENCONTRADO" : name) << std::endl;

        if (!req.has_file("file") || name.empty()) {
            res.status = 400;
            res.set_content("Falta el archivo o el nombre", "text/plain");
            return;
        }

        try {
            controller_->addDocument(name, req.get_file_value("file").content);
            std::string response = "Documento agregado: ";
            response += name;
            res.set_content(response, "text/plain");
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            std::string error = "Error: ";
            error += e.what();
            if (std::string(e.what()).find("ya existe") != std::string::npos) res.status = 409;
            else res.status = 500;
            res.set_content(error, "text/plain");
        }
        });

    // Endpoint para eliminar un documento
    svr_.Delete(R"(/documents/(.+))", [this](const httplib::Request& req, httplib::Response& res) {
        if (req.matches.size() > 1) {
            auto name = req.matches[1];
            try {
                controller_->deleteDocument(name);
                std::string response = "Documento eliminado: ";
                response += name;
                res.set_content(response, "text/plain");
            }
            catch (const std::exception& e) {
                std::string error = "Error: ";
                error += e.what();
                res.status = 404;
                res.set_content(error, "text/plain");
            }
        }
        else {
            res.status = 400;
            res.set_content("Nombre de documento no especificado", "text/plain");
        }
        });

    // Endpoint para buscar documentos
    svr_.Get("/documents", [this](const httplib::Request& req, httplib::Response& res) {
        auto name = req.get_param_value("name");
        auto exact = req.get_param_value("exact") == "true";
        try {
            auto results = controller_->searchDocument(name, exact);
            std::ostringstream oss;
            for (const auto& doc : results) {
                oss << doc << "\n";
            }
            res.set_content(oss.str(), "text/plain");
        }
        catch (const std::exception& e) {
            std::string error = "Error: ";
            error += e.what();
            res.status = 500;
            res.set_content(error, "text/plain");
        }
        });

    // Endpoint para descargar un documento
    svr_.Get(R"(/documents/(.+))", [this](const httplib::Request& req, httplib::Response& res) {
        if (req.matches.size() > 1) {
            auto name = req.matches[1];
            try {
                auto content = controller_->downloadDocument(name);
                // Detectar MIME type din�micamente (simplificado)
                std::string mime = controller_->downloadDocument(name); // Reutiliza para MIME
                res.set_content(content, mime.c_str());
            }
            catch (const std::exception& e) {
                std::string error = "Error: ";
                error += e.what();
                res.status = 404;
                res.set_content(error, "text/plain");
            }
        }
        else {
            res.status = 400;
            res.set_content("Nombre de documento no especificado", "text/plain");
        }
        });
}

void HttpServer::start() {
    std::cout << "Iniciando servidor en http://" << host_ << ":" << port_ << std::endl;
    svr_.listen(host_.c_str(), port_);
}