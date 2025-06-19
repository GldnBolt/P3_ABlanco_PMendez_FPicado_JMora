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
<<<<<<< HEAD
    int getBlockSize() const { return blockSize_; } // Nuevo getter
=======
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e

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

<<<<<<< HEAD
#endif // DISKNODE_H
=======
#endif // DISKNODE_H
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e
