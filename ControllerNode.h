#ifndef CONTROLLERNODE_H
#define CONTROLLERNODE_H

#include <vector>
#include <memory>
#include <string>
#include "DiskNode.h"

class ControllerNode {
public:
    // Recibe los 4 nodos de disco ya inicializados
    ControllerNode(const std::vector<std::shared_ptr<DiskNode>>& disks);

    // Escribe un bloque de datos + paridad en la "franja" stripeIndex
    void writeStripe(int stripeIndex, const std::string& data);

    // Lee un bloque de datos de la franja; si un disco falla, reconstruye
    std::string readStripe(int stripeIndex);

private:
    std::vector<std::shared_ptr<DiskNode>> disks_;
    int numDisks_;

    // Calcula el bloque de paridad XOR de todos los bloques dados
    std::string xorParity(const std::vector<std::string>& blocks);
};

#endif // CONTROLLERNODE_H
