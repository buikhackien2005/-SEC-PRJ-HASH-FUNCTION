#include "sha256_core.h"
#include "sha256_init.h"
#include <sstream>
#include <iomanip>

namespace sha256 {
// Các hàm logic đặc thù cho SHA-256 (32-bit)
namespace {
    inline uint32_t rotr(uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); }
    inline uint32_t Ch(uint32_t e, uint32_t f, uint32_t g) { return (e & f) ^ (~e & g); }
    inline uint32_t Maj(uint32_t a, uint32_t b, uint32_t c) { return (a & b) ^ (a & c) ^ (b & c); }
    inline uint32_t Sigma0(uint32_t a) { return rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22); }
    inline uint32_t Sigma1(uint32_t e) { return rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25); }
    inline uint32_t sigma0(uint32_t x) { return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3); }
    inline uint32_t sigma1(uint32_t x) { return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10); }
}

SHA256::SHA256() {
    for(int i=0; i<8; i++) state[i] = H0[i];
    buffer_length = 0; total_length = 0;
}

void SHA256::transform(const uint8_t* block) {
    uint32_t W[64];
    for (int t = 0; t < 16; ++t) {
        W[t] = (uint32_t)block[t*4] << 24 | (uint32_t)block[t*4+1] << 16 | 
               (uint32_t)block[t*4+2] << 8 | (uint32_t)block[t*4+3];
    }
    for (int t = 16; t < 64; ++t) {
        W[t] = sigma1(W[t-2]) + W[t-7] + sigma0(W[t-15]) + W[t-16];
    }

    uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
    uint32_t e = state[4], f = state[5], g = state[6], h = state[7];

    for (int t = 0; t < 64; ++t) {
        uint32_t T1 = h + Sigma1(e) + Ch(e, f, g) + K[t] + W[t];
        uint32_t T2 = Sigma0(a) + Maj(a, b, c);
        h = g; g = f; f = e; e = d + T1;
        d = c; c = b; b = a; a = T1 + T2;
    }
    state[0] += a; state[1] += b; state[2] += c; state[3] += d;
    state[4] += e; state[5] += f; state[6] += g; state[7] += h;
}

void SHA256::update(const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        buffer[buffer_length++] = data[i];
        if (buffer_length == 64) {
            transform(buffer);
            buffer_length = 0;
        }
    }
    total_length += length;
}

std::string SHA256::finalize() {
    uint64_t original_bits = total_length * 8;
    buffer[buffer_length++] = 0x80;
    if (buffer_length > 56) {
        while (buffer_length < 64) buffer[buffer_length++] = 0x00;
        transform(buffer);
        buffer_length = 0;
    }
    while (buffer_length < 56) buffer[buffer_length++] = 0x00;
    for (int i = 7; i >= 0; --i) {
        buffer[buffer_length++] = (original_bits >> (i * 8)) & 0xFF;
    }
    transform(buffer);
    std::stringstream ss;
    for (int i = 0; i < 8; ++i) ss << std::setfill('0') << std::setw(8) << std::hex << state[i];
    return ss.str();
}
}