#pragma once

#include "hash_common.h"

namespace sha512 {
std::string hex64(u64 x);
void print_bytes_hex(const std::vector<u8>& data, std::ostream& out, std::size_t perLine = 16);
std::vector<u8> preprocessSHA512(const std::string& msg, std::ostream& logFile, bool logPadding);
}
