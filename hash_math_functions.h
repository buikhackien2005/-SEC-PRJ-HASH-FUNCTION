#pragma once

#include "hash_common.h"

namespace sha512 {
u64 rotr(u64 x, int n);
u64 shr(u64 x, int n);
u64 Ch(u64 x, u64 y, u64 z);
u64 Maj(u64 x, u64 y, u64 z);
u64 BSIG0(u64 x);
u64 BSIG1(u64 x);
u64 SSIG0(u64 x);
u64 SSIG1(u64 x);
}
