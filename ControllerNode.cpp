#include "ControllerNode.h"
#include <stdexcept>

ControllerNode::ControllerNode(const std::vector<std::shared_ptr<DiskNode>>& disks)
  : disks_(disks), numDisks_(disks.size())
{
    if (numDisks_ < 3)
        throw std::runtime_error("RAID5 requiere al menos 3 discos");
}

std::string ControllerNode::xorParity(const std::vector<std::string>& blocks) {
    if (blocks.empty()) return "";
    size_t sz = blocks[0].size();
    std::string parity(sz, '\0');
    for (size_t i = 0; i < sz; ++i) {
        char p = 0;
        for (auto& blk : blocks) {
            p ^= blk[i];
        }
        parity[i] = p;
    }
    return parity;
}

void ControllerNode::writeStripe(int stripeIndex, const std::string& data) {
    // El disco de paridad rota: p = (numDisks_ - 1) - (stripeIndex % numDisks_)
    int p = (numDisks_ - 1) - (stripeIndex % numDisks_);
    std::vector<std::string> dataBlocks;
    dataBlocks.reserve(numDisks_ - 1);

    // Primero escribimos datos en todos menos en el de paridad
    for (int d = 0; d < numDisks_; ++d) {
        if (d == p) continue;
        disks_[d]->writeBlock(stripeIndex, data);
        dataBlocks.push_back(data);
    }
    // Luego calculamos y grabamos la paridad
    std::string parity = xorParity(dataBlocks);
    disks_[p]->writeBlock(stripeIndex, parity);
}

std::string ControllerNode::readStripe(int stripeIndex) {
    int p = (numDisks_ - 1) - (stripeIndex % numDisks_);
    std::vector<std::string> dataBlocks(numDisks_);
    int missing = -1;

    // Intentamos leer todos; detectamos si alguno falla (excepción)
    for (int d = 0; d < numDisks_; ++d) {
        try {
            dataBlocks[d] = disks_[d]->readBlock(stripeIndex);
        } catch (...) {
            missing = d;
        }
    }

    // Si todo OK, devolvemos el primero de datos (ejemplo)
    if (missing == -1) {
        for (int d = 0; d < numDisks_; ++d) {
            if (d != p) return dataBlocks[d];
        }
    }

    // Si hubo un fallo, reconstruimos:
    // block_missing = xorParity( todos excepto missing )
    std::vector<std::string> others;
    others.reserve(numDisks_ - 1);
    for (int d = 0; d < numDisks_; ++d) {
        if (d == missing) continue;
        others.push_back(dataBlocks[d]);
    }
    std::string recovered = xorParity(others);
    return recovered;
}
