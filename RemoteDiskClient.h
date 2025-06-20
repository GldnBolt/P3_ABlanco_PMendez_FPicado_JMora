//
// Created by Frand on 19/06/2025.
//

#pragma once
#include <string>

class RemoteDiskClient {
public:
    RemoteDiskClient(const std::string& ip, int port);

    std::string readBlock(int index);
    void writeBlock(int index, const std::string& data);
    int getBlockSize() const;

private:
    std::string ip_;
    int port_;

};
