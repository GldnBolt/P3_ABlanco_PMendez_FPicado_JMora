#include <iostream>
#include <memory>
#include "DiskNode.h"
#include "ControllerNode.h"
<<<<<<< HEAD
#include "HttpServer.h"
=======
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e

int main() {
    // Inicializa 4 nodos con sus configs
    std::vector<std::shared_ptr<DiskNode>> disks;
    for (int i = 1; i <= 4; ++i) {
        disks.push_back(
<<<<<<< HEAD
            std::make_shared<DiskNode>("config/node" + std::to_string(i) + ".xml")
        );
    }

    // Crear ControllerNode con archivo de metadatos configurable
    auto controller = std::make_shared<ControllerNode>("data/metadata/metadata.txt", disks);

    // Iniciar servidor HTTP
    HttpServer server(controller, "localhost", 8080);
    std::cout << "Servidor HTTP iniciado en http://localhost:8080" << std::endl;
    server.start();

    return 0;
}
=======
          std::make_shared<DiskNode>("config/node" + std::to_string(i) + ".xml")
        );
    }

    ControllerNode ctrl(disks);
    std::string msg = "Hola RAID5 Striped";

    // Escribe y lee la franja 0 sin fallos
    ctrl.writeStripe(0, msg);
    std::cout << "Leído sin fallo: "
              << ctrl.readStripe(0).substr(0, msg.size())
              << std::endl;

    // Simula fallo: borra o renombra un block_0.dat en data/node2/
    // Luego:
    std::cout << "Leído con un fallo simulado: "
              << ctrl.readStripe(0).substr(0, msg.size())
              << std::endl;

    return 0;
}
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e
