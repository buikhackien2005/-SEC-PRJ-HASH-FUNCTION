#pragma once

#include "hash_common.h"

namespace sha512 {
const std::array<u64, 80>& getK();
const std::array<u64, 8>& getH0();
}
