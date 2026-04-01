#pragma once
#include <bits/stdc++.h>
using namespace std;

// 64 bits to 16 bytes
string hex64(u64 x) {
    stringstream ss;
    ss << hex << nouppercase << setfill('0') << setw(16) << x;
    return ss.str();
}

// Hàm in ra các giá trị trung gian
void print_bytes_hex(const vector<u8>& data, ostream& out, size_t perLine = 16) {
    for (size_t i = 0; i < data.size(); ++i) {
        out << hex << nouppercase << setfill('0') << setw(2)
            << static_cast<unsigned>(data[i]) << " ";
        if ((i + 1) % perLine == 0) out << "\n";
    }
    if (data.size() % perLine != 0) out << "\n";
    out << dec;
}

// I, Padding & Parsing function (Skeleton)
vector<u8> PaddingParsing_SHA512(const string& msg) {
    // Sao chép nội dung chuỗi vào vector byte
    vector<u8> data(msg.begin(), msg.end());

    // TODO: Implement padding logic here

    return data;
}