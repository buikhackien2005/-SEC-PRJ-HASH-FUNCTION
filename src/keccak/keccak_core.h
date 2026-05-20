#ifndef KECCAK_CORE_H
#define KECCAK_CORE_H

#include "../utils/common.h"
#include <string>

namespace keccak {
class Keccak {
public:
    Keccak();
    void update(const uint8_t* data, size_t length);
    std::string finalize();

private:
    uint64_t state[25];        // Trạng thái 1600-bit (25 * 64-bit)
    uint8_t buffer[136];       // Rate của SHA3-256 là 136 bytes
    size_t buffer_length;

    void keccak_f1600();       // Hàm nhào trộn cốt lõi (24 vòng)
};
}

#endif