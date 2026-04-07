#include "hash_padding_parsing.h"

namespace sha512 {
std::string hex64(u64 x) {
    std::stringstream ss;
    ss << std::hex << std::nouppercase << std::setfill('0') << std::setw(16) << x;
    return ss.str();
}

void print_bytes_hex(const std::vector<u8>& data, std::ostream& out, std::size_t perLine) {
    for (size_t i = 0; i < data.size(); ++i) {
        out << std::hex << std::nouppercase << std::setfill('0') << std::setw(2)
            << static_cast<unsigned>(data[i]) << " ";
        if ((i + 1) % perLine == 0) out << "\n";
    }
    if (data.size() % perLine != 0) out << "\n";
    out << std::dec;
}

// I, Padding & Parsing function (Core Padding)
std::vector<u8> preprocessSHA512(const std::string& msg, std::ostream& logFile, bool logPadding) {
    // Sao chép nội dung chuỗi vào vector byte
    std::vector<u8> data(msg.begin(), msg.end());

    // SHA-512 biểu diễn độ dài thông điệp bằng 128 bit, do ko có số nguyên 128 bit nên chia thành high 64 bit và low 64 bit.
    // Trong code này, thông điệp nhỏ nên phần high = 0.
    u64 bit_len_high = 0;
    u64 bit_len_low  = static_cast<u64>(data.size()) * 8ULL;

    // Ghi log thông tin đầu vào
    if (logPadding) {
        logFile << "========== [1] INPUT ==========\n";
        logFile << "Chuoi goc: \"" << msg << "\"\n";
        logFile << "So byte ban dau: " << data.size() << "\n";
        logFile << "Do dai bit ban dau: " << bit_len_low << "\n\n";
    }

    // Thêm 1 bit '1' và 7 bit '0' => tương đương byte 0x80
    data.push_back(0x80);

    // Thêm các byte 0x00 cho đến khi:
    // (độ dài hiện tại + 16 byte độ dài cuối) chia hết cho 128 byte
    // vì mỗi block SHA-512 có kích thước 1024 bit = 128 byte
    while ((data.size() + 16) % 128 != 0) {
        data.push_back(0x00);
    }

    // Gắn 64 bit cao của độ dài ban đầu vào cuối dữ liệu
    for (int i = 7; i >= 0; --i) {
        data.push_back(static_cast<u8>((bit_len_high >> (i * 8)) & 0xFF));
    }

    // Gắn 64 bit thấp của độ dài ban đầu vào cuối dữ liệu
    for (int i = 7; i >= 0; --i) {
        data.push_back(static_cast<u8>((bit_len_low >> (i * 8)) & 0xFF));
    }

    // Ghi log sau khi padding
    if (logPadding) {
        logFile << "========== [2] SAU PADDING ==========\n";
        logFile << "Tong so byte sau padding: " << data.size() << "\n";
        logFile << "So block 1024-bit: " << data.size() / 128 << "\n";
        logFile << "Du lieu sau padding (hex):\n";
        print_bytes_hex(data, logFile);
        logFile << "\n";
    }

    return data;
}
}
