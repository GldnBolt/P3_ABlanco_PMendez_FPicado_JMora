#ifndef TECMFS_DISK_H
#define TECMFS_DISK_H

#include <string>
#include <vector>

class DiskNode {
public:
    DiskNode(std::string ip, int port, std::string storagePath);
    ~DiskNode();

    // Método para leer datos de un bloque
    std::string readBlock(int blockIndex);

    // Método para escribir datos en un bloque
    void writeBlock(int blockIndex, const std::string& data);

    // Método para obtener la información del nodo
    std::string getInfo() const;

private:
    std::string ip_;         // Dirección IP del nodo
    int port_;               // Puerto de comunicación
    std::string storagePath_;// Ruta de almacenamiento en el sistema de archivos
    std::vector<std::string> blocks_;  // Bloques de datos en el nodo
    void loadConfiguration(const std::string& configPath);
};

#endif
