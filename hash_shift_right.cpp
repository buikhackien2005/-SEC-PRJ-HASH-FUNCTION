#include "hash_math_functions.h"

namespace sha512 {
// right shifting function
u64 shr(u64 x, int n) {
    return x >> n;
}
}