#ifndef SHA512_CORE_H
#define SHA512_CORE_H

#include "../utils/common.h"

namespace sha512 {

class SHA512 {
public:
    SHA512();
    
    // Nạp dữ liệu theo từng khối (chunk)
    void update(const uint8_t* data, size_t length);
    
    // Kết thúc băm, tự động xử lý padding và trả về chuỗi Hex
    std::string finalize();

private:
    uint64_t state[8];         // Thanh ghi H0 - H7
    uint8_t buffer[128];       // Bộ đệm chứa dữ liệu chưa đủ block 128 byte
    size_t buffer_length;      // Số byte hiện có trong bộ đệm
    uint64_t total_length;     // Tổng số byte đã xử lý

    // Hàm xử lý toán học lõi cho 1 block 1024-bit
    void transform(const uint8_t* block);
};

}

#endif // SHA512_CORE_H