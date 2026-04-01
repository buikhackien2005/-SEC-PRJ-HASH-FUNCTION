#pragma once
#include <bits/stdc++.h>
using namespace std;

string hex64(u64 x) {
    stringstream ss;
    ss << hex << nouppercase << setfill('0') << setw(16) << x;
    return ss.str();
}

void print_bytes_hex(const vector<u8>& data, ostream& out, size_t perLine = 16) {
    for (size_t i = 0; i < data.size(); ++i) {
        out << hex << nouppercase << setfill('0') << setw(2)
            << static_cast<unsigned>(data[i]) << " ";
        if ((i + 1) % perLine == 0) out << "\n";
    }
    if (data.size() % perLine != 0) out << "\n";
    out << dec;
}

// I, Padding & Parsing function (Core Padding)
vector<u8> PaddingParsing_SHA512(const string& msg) {
    vector<u8> data(msg.begin(), msg.end());

    // Thêm 1 bit '1' và 7 bit '0' => tương đương byte 0x80
    data.push_back(0x80);

    // Thêm các byte 0x00 cho đến khi:
    // (độ dài hiện tại + 16 byte độ dài cuối) chia hết cho 128 byte
    // vì mỗi block SHA-512 có kích thước 1024 bit = 128 byte
    while ((data.size() + 16) % 128 != 0) {
        data.push_back(0x00);
    }

    // TODO: Append original message length

    return data;
}