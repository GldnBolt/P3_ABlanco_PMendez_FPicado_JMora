#include <iostream>
#include <memory>
#include "DiskNode.h"
#include "ControllerNode.h"

int main() {
    // Inicializa 4 nodos con sus configs
    std::vector<std::shared_ptr<DiskNode>> disks;
    for (int i = 1; i <= 4; ++i) {
        disks.push_back(
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
