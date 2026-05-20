#include "sha1_core.h"
#include <sstream>
#include <iomanip>

namespace sha1 {

namespace {
    inline uint32_t rotl(uint32_t value, unsigned int count) {
        return (value << count) | (value >> (32 - count));
    }
}

SHA1::SHA1() {
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
    state[4] = 0xc3d2e1f0;
    buffer_length = 0;
    total_length = 0;
}

void SHA1::transform(const uint8_t* block) {
    uint32_t W[80];

    // Đọc 16 từ 32-bit đầu tiên theo chuẩn Big-Endian
    for (int i = 0; i < 16; i++) {
        W[i] = (block[i * 4] << 24) | (block[i * 4 + 1] << 16) | 
               (block[i * 4 + 2] << 8) | (block[i * 4 + 3]);
    }

    // Mở rộng mảng W cho 64 vòng còn lại
    for (int i = 16; i < 80; i++) {
        W[i] = rotl(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
    }

    uint32_t a = state[0], b = state[1], c = state[2], d = state[3], e = state[4];

    for (int i = 0; i < 80; i++) {
        uint32_t f, k;
        if (i < 20) {
            f = (b & c) | ((~b) & d);
            k = 0x5a827999;
        } else if (i < 40) {
            f = b ^ c ^ d;
            k = 0x6ed9eba1;
        } else if (i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8f1bbcdc;
        } else {
            f = b ^ c ^ d;
            k = 0xca62c1d6;
        }

        uint32_t temp = rotl(a, 5) + f + e + k + W[i];
        e = d;
        d = c;
        c = rotl(b, 30);
        b = a;
        a = temp;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

void SHA1::update(const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        buffer[buffer_length++] = data[i];
        if (buffer_length == 64) {
            transform(buffer);
            buffer_length = 0;
        }
    }
    total_length += length;
}

std::string SHA1::finalize() {
    uint64_t original_bits = total_length * 8;
    
    // Thêm bit 1 (0x80)
    buffer[buffer_length++] = 0x80;

    // Nếu không đủ chỗ ghi 8 bytes độ dài, lấp đầy 0 và băm trước block này
    if (buffer_length > 56) {
        while (buffer_length < 64) buffer[buffer_length++] = 0x00;
        transform(buffer);
        buffer_length = 0;
    }

    // Lấp đầy 0 cho đến byte 56
    while (buffer_length < 56) buffer[buffer_length++] = 0x00;

    // Ghi độ dài 8 byte bằng Big-Endian
    for (int i = 7; i >= 0; --i) {
        buffer[buffer_length++] = (original_bits >> (i * 8)) & 0xFF;
    }

    transform(buffer);

    // Xuất chuỗi Hex
    std::stringstream ss;
    for (int i = 0; i < 5; ++i) {
        ss << std::setfill('0') << std::setw(8) << std::hex << state[i];
    }
    return ss.str();
}

}