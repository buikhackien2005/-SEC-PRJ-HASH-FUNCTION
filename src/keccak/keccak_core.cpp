#include "keccak_core.h"
#include <sstream>
#include <iomanip>
#include <cstring>

namespace keccak {

// Các hằng số vòng (Round Constants) của Keccak
const uint64_t RC[24] = {
    0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
    0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
    0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
    0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
    0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
};

namespace {
    inline uint64_t rotl(uint64_t x, int y) {
        return (x << y) | (x >> (64 - y));
    }
}

Keccak::Keccak() {
    memset(state, 0, sizeof(state));
    buffer_length = 0;
}

void Keccak::keccak_f1600() {
    for (int round = 0; round < 24; ++round) {
        // Theta
        uint64_t C[5], D[5];
        for (int i = 0; i < 5; ++i)
            C[i] = state[i] ^ state[i + 5] ^ state[i + 10] ^ state[i + 15] ^ state[i + 20];
        for (int i = 0; i < 5; ++i)
            D[i] = C[(i + 4) % 5] ^ rotl(C[(i + 1) % 5], 1);
        for (int i = 0; i < 25; ++i)
            state[i] ^= D[i % 5];

        // Rho & Pi
        uint64_t x = 1, y = 0, current = state[1];
        for (int i = 0; i < 24; ++i) {
            int t = x;
            x = y;
            y = (2 * t + 3 * y) % 5;
            uint64_t temp = state[x + 5 * y];
            state[x + 5 * y] = rotl(current, ((i + 1) * (i + 2) / 2) % 64);
            current = temp;
        }

        // Chi
        for (int j = 0; j < 25; j += 5) {
            uint64_t temp[5];
            for (int i = 0; i < 5; ++i)
                temp[i] = state[j + i];
            for (int i = 0; i < 5; ++i)
                state[j + i] ^= (~temp[(i + 1) % 5]) & temp[(i + 2) % 5];
        }

        // Iota
        state[0] ^= RC[round];
    }
}

void Keccak::update(const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        buffer[buffer_length++] = data[i];
        if (buffer_length == 136) {
            // Hấp thụ (Absorb) vào trạng thái bằng XOR (Little-Endian)
            for (int j = 0; j < 17; ++j) { // 136 bytes = 17 * 8 bytes
                uint64_t lane = 0;
                for (int k = 0; k < 8; ++k) lane |= ((uint64_t)buffer[j * 8 + k]) << (8 * k);
                state[j] ^= lane;
            }
            keccak_f1600();
            buffer_length = 0;
        }
    }
}

std::string Keccak::finalize() {
    // Đệm (Padding) chuẩn SHA3-256: Thêm 0x06
    // (Nếu muốn dùng chuẩn Ethereum Keccak, thay 0x06 bằng 0x01)
    buffer[buffer_length++] = 0x06; 

    // Lấp đầy 0x00 cho đến sát byte cuối cùng
    while (buffer_length < 135) buffer[buffer_length++] = 0x00;

    // Byte cuối cùng luôn được XOR với 0x80 (Bit 1 ở vị trí cao nhất)
    buffer[135] = 0x80;

    // Absorb block cuối
    for (int j = 0; j < 17; ++j) {
        uint64_t lane = 0;
        for (int k = 0; k < 8; ++k) lane |= ((uint64_t)buffer[j * 8 + k]) << (8 * k);
        state[j] ^= lane;
    }
    keccak_f1600();

    // Vắt (Squeeze) ra 32 bytes (256 bits) mã băm
    std::stringstream ss;
    for (int i = 0; i < 4; ++i) { // 4 * 8 = 32 bytes
        uint64_t lane = state[i];
        for (int k = 0; k < 8; ++k) {
            ss << std::setfill('0') << std::setw(2) << std::hex << (int)((lane >> (8 * k)) & 0xFF);
        }
    }
    return ss.str();
}

}