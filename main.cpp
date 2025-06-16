#include <iostream>
#include "DiskNode.h"

int main() {
    DiskNode node("D:/GitHub/P3_ABlanco_PMendez_FPicado_JMora/config/config.xml");

    std::cout << node.getInfo() << std::endl;

    auto blk0 = node.readBlock(0);
    node.writeBlock(0, "Hola RAID5");
    auto blk0b = node.readBlock(0);
    std::cout << "Primera línea de block0: "
              << blk0b.substr(0,10)
              << std::endl;

    return 0;
}
