#include "hash_math_functions.h"

namespace sha512 {
// right bit rotation function
u64 rotr(u64 x, int n) {
    return (x >> n) | (x << (64 - n));
}
}