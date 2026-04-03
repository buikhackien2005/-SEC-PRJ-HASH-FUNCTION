#pragma once
#include <bits/stdc++.h>
using namespace std;

// Hàm chính xử lý SHA-512 và ghi các giá trị trung gian ra file .txt
string sha512_trace_to_file(const string& msg,
                            ostream& logFile,
                            bool logPadding = true,
                            bool logSchedule = true,
                            bool logRounds = true,
                            bool logHEachBlock = true) {
    // Tiền xử lý dữ liệu
    vector<u8> data = PaddingParsing_SHA512(msg, logFile, logPadding);

    // Khởi tạo H bằng giá trị ban đầu H0
    array<u64, 8> H = H0;

    // Số block 1024-bit cần xử lý
    size_t numBlocks = data.size() / 128;

    // Duyệt từng block
    for (size_t blockIndex = 0; blockIndex < numBlocks; ++blockIndex) {
        logFile << "========================================\n";
        logFile << "BLOCK " << blockIndex << "\n";
        logFile << "========================================\n";

        // W là message schedule gồm 80 word 64-bit
        array<u64, 80> W{};

        // offset = vị trí byte bắt đầu của block hiện tại trong mảng data
        size_t offset = blockIndex * 128;

        // Đọc 16 word đầu tiên W[0..15] trực tiếp từ block
        // Mỗi word gồm 8 byte = 64 bit
        for (int t = 0; t < 16; ++t) {
            u64 word = 0;
            for (int j = 0; j < 8; ++j) {
                word = (word << 8) | data[offset + t * 8 + j];
            }
            W[t] = word;
        }

        // Ghi log 16 word đầu tiên
        logFile << "[3] 16 WORD DAU TU BLOCK:\n";
        for (int t = 0; t < 16; ++t) {
            logFile << "W[" << setw(2) << setfill('0') << t << "] = 0x" << hex64(W[t]) << "\n";
        }
        logFile << "\n";

        // Nội suy W[16..79] theo công thức chuẩn SHA-512
        for (int t = 16; t < 80; ++t) {
            W[t] = SSIG1(W[t - 2]) + W[t - 7] + SSIG0(W[t - 15]) + W[t - 16];
        }

        // Ghi log toàn bộ message schedule nếu được yêu cầu
        if (logSchedule) {
            logFile << "[4] MESSAGE SCHEDULE W[16..79]:\n";
            for (int t = 16; t < 80; ++t) {
                logFile << "W[" << setw(2) << setfill('0') << t << "] = 0x" << hex64(W[t]) << "\n";
            }
            logFile << "\n";
        }

        // Sao chép giá trị H hiện tại vào 8 biến làm việc a..h
        // Đây là 8 thanh ghi nội bộ của bước nén.
        u64 a = H[0], b = H[1], c = H[2], d = H[3];
        u64 e = H[4], f = H[5], g = H[6], h = H[7];

        // Ghi log trạng thái ban đầu trước 80 round
        logFile << "[5] GIA TRI KHOI TAO a..h TRUOC 80 ROUND:\n";
        logFile << "a = " << hex64(a) << "\n";
        logFile << "b = " << hex64(b) << "\n";
        logFile << "c = " << hex64(c) << "\n";
        logFile << "d = " << hex64(d) << "\n";
        logFile << "e = " << hex64(e) << "\n";
        logFile << "f = " << hex64(f) << "\n";
        logFile << "g = " << hex64(g) << "\n";
        logFile << "h = " << hex64(h) << "\n\n";

        // Thực hiện 80 round nén
        for (int t = 0; t < 80; ++t) {
            // T1 = h + Σ1(e) + Ch(e,f,g) + K[t] + W[t]
            u64 T1 = h + BSIG1(e) + Ch(e, f, g) + K[t] + W[t];

            // T2 = Σ0(a) + Maj(a,b,c)
            u64 T2 = BSIG0(a) + Maj(a, b, c);

            // Ghi log trước khi cập nhật a..h
            if (logRounds) {
                logFile << "----- ROUND " << setw(2) << setfill('0') << t << " -----\n";
                logFile << "W[" << t << "] = " << hex64(W[t]) << "\n";
                logFile << "K[" << t << "] = " << hex64(K[t]) << "\n";
                logFile << "T1 = " << hex64(T1) << "\n";
                logFile << "T2 = " << hex64(T2) << "\n";

                logFile << "Before:\n";
                logFile << "a=" << hex64(a) << " b=" << hex64(b)
                        << " c=" << hex64(c) << " d=" << hex64(d) << "\n";
                logFile << "e=" << hex64(e) << " f=" << hex64(f)
                        << " g=" << hex64(g) << " h=" << hex64(h) << "\n";
            }

            // Tính giá trị mới cho a và e
            u64 new_a = T1 + T2;
            u64 new_e = d + T1;

            // Dịch chuyển 8 thanh ghi theo quy tắc của SHA-512
            h = g;
            g = f;
            f = e;
            e = new_e;
            d = c;
            c = b;
            b = a;
            a = new_a;

            // Ghi log sau khi cập nhật
            if (logRounds) {
                logFile << "After:\n";
                logFile << "a=" << hex64(a) << " b=" << hex64(b)
                        << " c=" << hex64(c) << " d=" << hex64(d) << "\n";
                logFile << "e=" << hex64(e) << " f=" << hex64(f)
                        << " g=" << hex64(g) << " h=" << hex64(h) << "\n\n";
            }
        }

        // Sau khi xử lý xong 1 block:
        // cộng dồn kết quả a..h vào H[0..7]
        H[0] += a; H[1] += b; H[2] += c; H[3] += d;
        H[4] += e; H[5] += f; H[6] += g; H[7] += h;

        // Ghi log H sau khi xử lý block hiện tại
        if (logHEachBlock) {
            logFile << "[6] H SAU KHI XU LY XONG BLOCK " << blockIndex << ":\n";
            for (int i = 0; i < 8; ++i) {
                logFile << "H[" << i << "] = " << hex64(H[i]) << "\n";
            }
            logFile << "\n";
        }
    }

    // Ghép 8 word H[0..7] thành chuỗi hash cuối cùng
    stringstream digest;
    for (u64 x : H) {
        digest << hex64(x);
    }

    return digest.str();
}
