#ifndef CONTROLLERNODE_H
#define CONTROLLERNODE_H

#include <vector>
#include <memory>
#include <string>
<<<<<<< HEAD
#include <map>
=======
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e
#include "DiskNode.h"

class ControllerNode {
public:
<<<<<<< HEAD
    ControllerNode(const std::string& metadataFile, const std::vector<std::shared_ptr<DiskNode>>& disks);

    void writeStripe(int stripeIndex, const std::string& data);
    std::string readStripe(int stripeIndex);

    // Nuevas funciones para documentos
    void addDocument(const std::string& docName, const std::string& content);
    void deleteDocument(const std::string& docName);
    std::vector<std::string> searchDocument(const std::string& docName, bool exact = false);
    std::string downloadDocument(const std::string& docName);
=======
    // Recibe los 4 nodos de disco ya inicializados
    ControllerNode(const std::vector<std::shared_ptr<DiskNode>>& disks);

    // Escribe un bloque de datos + paridad en la "franja" stripeIndex
    void writeStripe(int stripeIndex, const std::string& data);

    // Lee un bloque de datos de la franja; si un disco falla, reconstruye
    std::string readStripe(int stripeIndex);
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e

private:
    std::vector<std::shared_ptr<DiskNode>> disks_;
    int numDisks_;
<<<<<<< HEAD
    std::map<std::string, std::vector<int>> docToStripes_; // Mapa de documento a franjas
    std::string metadataFile_;

    std::string xorParity(const std::vector<std::string>& blocks);
    void loadMetadata();
    void saveMetadata();
};

#endif // CONTROLLERNODE_H
=======

    // Calcula el bloque de paridad XOR de todos los bloques dados
    std::string xorParity(const std::vector<std::string>& blocks);
};

#endif // CONTROLLERNODE_H
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e
