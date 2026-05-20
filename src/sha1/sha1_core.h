#ifndef SHA1_CORE_H
#define SHA1_CORE_H

#include "../utils/common.h"
#include <string>

namespace sha1 {
class SHA1 {
public:
    SHA1();
    void update(const uint8_t* data, size_t length);
    std::string finalize();

private:
    uint32_t state[5];         // 5 thanh ghi: A, B, C, D, E
    uint8_t buffer[64];        // Kích thước khối là 64 bytes
    size_t buffer_length;
    uint64_t total_length;

    void transform(const uint8_t* block);
};
}

#endif