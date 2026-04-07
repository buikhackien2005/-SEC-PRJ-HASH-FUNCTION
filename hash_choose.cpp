#include "hash_math_functions.h"

namespace sha512 {
// Choose function
u64 Ch(u64 x, u64 y, u64 z) {
    return (x & y) ^ (~x & z);
}
}