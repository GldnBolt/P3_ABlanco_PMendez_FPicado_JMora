#ifndef CONTROLLERNODE_H
#define CONTROLLERNODE_H

#include <vector>
#include <memory>
#include <string>
#include <map>
#include "DiskNode.h"

class ControllerNode {
public:
    ControllerNode(const std::string& metadataFile, const std::vector<std::shared_ptr<DiskNode>>& disks);

    void writeStripe(int stripeIndex, const std::string& data);
    std::string readStripe(int stripeIndex);

    // Nuevas funciones para documentos
    void addDocument(const std::string& docName, const std::string& content);
    void deleteDocument(const std::string& docName);
    std::vector<std::string> searchDocument(const std::string& docName, bool exact = false);
    std::string downloadDocument(const std::string& docName);

private:
    std::vector<std::shared_ptr<DiskNode>> disks_;
    int numDisks_;
    std::map<std::string, std::vector<int>> docToStripes_;
    std::string metadataFile_;

    std::string xorParity(const std::vector<std::string>& blocks);
    void loadMetadata();
    void saveMetadata();
};

#endif // CONTROLLERNODE_H