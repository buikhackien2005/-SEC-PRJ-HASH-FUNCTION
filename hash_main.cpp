#include "hash_80_rounds.h"
#include "hash_common.h"

#ifndef SHA512_SEPARATE_BUILD
#include "hash_80_rounds.cpp"
#include "hash_padding_parsing.cpp"
#include "hash_initial_values.cpp"
#include "hash_rotr.cpp"
#include "hash_shift_right.cpp"
#include "hash_choose.cpp"
#include "hash_majority.cpp"
#include "hash_big_sigma0.cpp"
#include "hash_big_sigma1.cpp"
#include "hash_small_sigma0.cpp"
#include "hash_small_sigma1.cpp"
#endif

int main() {
    std::string msg;

    // Nhập chuỗi cần băm từ bàn phím
    std::cout << "Nhap chuoi can bam: ";
    std::getline(std::cin, msg);

    // Mở file output.txt để ghi toàn bộ log trung gian
    std::ofstream fout("output.txt");
    if (!fout.is_open()) {
        std::cerr << "Khong mo duoc file output.txt\n";
        return 1;
    }

    // Gọi hàm băm SHA-512 và bật toàn bộ log:
    // - log padding
    // - log message schedule
    // - log từng round
    // - log H sau mỗi block
    std::string hash = sha512::sha512_trace_to_file(
        msg,
        fout,
        true,   // ghi padding vao file
        true,   // ghi W[16..79] vao file
        true,   // ghi tung round vao file
        true    // ghi H sau moi block vao file
    );

    // Ghi kết quả hash cuối cùng vào file
    fout << "========== [7] KET QUA CUOI ==========\n";
    fout << "SHA-512 = " << hash << "\n";
    fout.close();

    // In kết quả ra màn hình
    std::cout << "\nSHA-512 = " << hash << "\n";
    std::cout << "Da ghi cac buoc trung gian vao file output.txt\n";

    return 0;
}
