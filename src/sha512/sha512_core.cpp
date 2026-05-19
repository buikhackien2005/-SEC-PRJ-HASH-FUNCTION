#include "sha512_core.h"
#include "sha512_init.h"
#include "../utils/bit_operations.h"
#include <sstream>
#include <iomanip>

namespace sha512 {

namespace {
    inline uint64_t Ch(uint64_t e, uint64_t f, uint64_t g) { return (e & f) ^ (~e & g); }
    inline uint64_t Maj(uint64_t a, uint64_t b, uint64_t c) { return (a & b) ^ (a & c) ^ (b & c); }
    inline uint64_t BigSigma0(uint64_t a) { return rotr(a, 28) ^ rotr(a, 34) ^ rotr(a, 39); }
    inline uint64_t BigSigma1(uint64_t e) { return rotr(e, 14) ^ rotr(e, 18) ^ rotr(e, 41); }
    inline uint64_t SmallSigma0(uint64_t x) { return rotr(x, 1) ^ rotr(x, 8) ^ shr(x, 7); }
    inline uint64_t SmallSigma1(uint64_t x) { return rotr(x, 19) ^ rotr(x, 61) ^ shr(x, 6); }
}

SHA512::SHA512() {
    for (int i = 0; i < 8; ++i) state[i] = H0[i];
    buffer_length = 0;
    total_length = 0;
}

void SHA512::transform(const uint8_t* block) {
    uint64_t W[80] = {0};

    // Parse 128 bytes thành 16 words 64-bit (Big-Endian)
    for (int t = 0; t < 16; ++t) {
        size_t offset = t * 8;
        W[t] = (static_cast<uint64_t>(block[offset]) << 56) |
               (static_cast<uint64_t>(block[offset + 1]) << 48) |
               (static_cast<uint64_t>(block[offset + 2]) << 40) |
               (static_cast<uint64_t>(block[offset + 3]) << 32) |
               (static_cast<uint64_t>(block[offset + 4]) << 24) |
               (static_cast<uint64_t>(block[offset + 5]) << 16) |
               (static_cast<uint64_t>(block[offset + 6]) << 8) |
               (static_cast<uint64_t>(block[offset + 7]));
    }

    for (int t = 16; t < 80; ++t) {
        W[t] = SmallSigma1(W[t - 2]) + W[t - 7] + SmallSigma0(W[t - 15]) + W[t - 16];
    }

    uint64_t a = state[0], b = state[1], c = state[2], d = state[3];
    uint64_t e = state[4], f = state[5], g = state[6], h = state[7];

    for (int t = 0; t < 80; ++t) {
        uint64_t T1 = h + BigSigma1(e) + Ch(e, f, g) + K[t] + W[t];
        uint64_t T2 = BigSigma0(a) + Maj(a, b, c);
        h = g; g = f; f = e; e = d + T1;
        d = c; c = b; b = a; a = T1 + T2;
    }

    state[0] += a; state[1] += b; state[2] += c; state[3] += d;
    state[4] += e; state[5] += f; state[6] += g; state[7] += h;
}

void SHA512::update(const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        buffer[buffer_length++] = data[i];
        if (buffer_length == 128) {
            transform(buffer);
            buffer_length = 0;
        }
    }
    total_length += length;
}

std::string SHA512::finalize() {
    uint64_t original_bits = total_length * 8;

    // Thêm byte 0x80 (bit 1)
    buffer[buffer_length++] = 0x80;

    // Nếu không đủ chỗ cho 16 bytes độ dài, lấp đầy block hiện tại rồi băm trước
    if (buffer_length > 112) {
        while (buffer_length < 128) buffer[buffer_length++] = 0x00;
        transform(buffer);
        buffer_length = 0;
    }

    // Lấp đầy 0x00 cho đến khi dư đúng 16 byte cuối
    while (buffer_length < 112) buffer[buffer_length++] = 0x00;

    // 8 byte đầu của độ dài (luôn bằng 0 cho các luồng dữ liệu nhỏ hơn 2^61 bytes)
    for (int i = 0; i < 8; ++i) buffer[buffer_length++] = 0x00;
    
    // 8 byte sau ghi nhận độ dài theo Big-Endian
    for (int i = 7; i >= 0; --i) {
        buffer[buffer_length++] = static_cast<uint8_t>((original_bits >> (i * 8)) & 0xFF);
    }

    transform(buffer);

    // Xuất Hex
    std::stringstream ss;
    for (int i = 0; i < 8; ++i) {
        ss << std::setfill('0') << std::setw(16) << std::hex << state[i];
    }
    return ss.str();
}

} // namespace sha512