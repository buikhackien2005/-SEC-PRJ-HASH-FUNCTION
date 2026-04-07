#include "hash_math_functions.h"

namespace sha512 {
// Majority function
u64 Maj(u64 x, u64 y, u64 z) {
    return (x & y) ^ (x & z) ^ (y & z);
}
}