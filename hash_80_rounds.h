#pragma once

#include "hash_common.h"

namespace sha512 {
std::string sha512_trace_to_file(const std::string& msg,
                                 std::ostream& logFile,
                                 bool logPadding = true,
                                 bool logSchedule = true,
                                 bool logRounds = true,
                                 bool logHEachBlock = true);
}
