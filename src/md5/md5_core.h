#ifndef MD5_CORE_H
#define MD5_CORE_H

#include "../utils/common.h"
#include <string>

namespace md5 {
class MD5 {
public:
    MD5();
    void update(const uint8_t* data, size_t length);
    std::string finalize();

private:
    uint32_t state[4];         // A, B, C, D
    uint8_t buffer[64];        // MD5 dùng block 64 bytes
    size_t buffer_length;
    uint64_t total_length;     // Lưu số BYTE đã đọc

    void transform(const uint8_t* block);
};
}

#endif