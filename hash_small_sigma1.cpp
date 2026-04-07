#include "hash_math_functions.h"

namespace sha512 {
// Small_Sigma_1 function used to duplicate W[i]
u64 SSIG1(u64 x) {
    return rotr(x, 19) ^ rotr(x, 61) ^ shr(x, 6);
}
}