#ifndef SHA256_CORE_H
#define SHA256_CORE_H

#include "../utils/common.h"

namespace sha256 {
class SHA256 {
public:
    SHA256();
    void update(const uint8_t* data, size_t length);
    std::string finalize();
private:
    uint32_t state[8];
    uint8_t buffer[64]; // Block size của SHA-256 là 64 bytes
    size_t buffer_length;
    uint64_t total_length;
    void transform(const uint8_t* block);
};
}
#endif