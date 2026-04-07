#include "hash_primitives_math.cpp"
#include "hash_padding_parsing.cpp"
#include "hash_80_rounds.cpp"

#define u8 uint8_t
#define u64 uint64_t

int main() {
    string msg;

    // Nhập chuỗi cần băm từ bàn phím
    cout << "Nhap chuoi can bam: ";
    getline(cin, msg);

    // Mở file output.txt để ghi toàn bộ log trung gian
    ofstream fout("output.txt");
    if (!fout.is_open()) {
        cerr << "Khong mo duoc file output.txt\n";
        return 1;
    }

    // Gọi hàm băm SHA-512 và bật toàn bộ log:
    // - log padding
    // - log message schedule
    // - log từng round
    // - log H sau mỗi block
    string hash = sha512_trace_to_file(
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
    cout << "\nSHA-512 = " << hash << "\n";
    cout << "Da ghi cac buoc trung gian vao file output.txt\n";

    return 0;
}
