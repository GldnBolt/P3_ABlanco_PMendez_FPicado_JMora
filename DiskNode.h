#ifndef DISKNODE_H
#define DISKNODE_H

#include <string>
#include <vector>

class DiskNode {
public:
    DiskNode(const std::string& configPath);
    ~DiskNode();

    std::string readBlock(int blockIndex);
    void writeBlock(int blockIndex, const std::string& data);
    std::string getInfo() const;

private:
    std::string ip_;
    int port_;
    std::string storageDir_;
    int numBlocks_;
    int blockSize_;
    std::vector<std::string> blocks_;

    void loadConfiguration(const std::string& configPath);
    void initializeStorage();
    std::string blockFilename(int blockIndex) const;
};

#endif // DISKNODE_H
