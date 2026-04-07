#include "hash_math_functions.h"

namespace sha512 {
// Small_Sigma_0 function used to duplicate W[i]
u64 SSIG0(u64 x) {
    return rotr(x, 1) ^ rotr(x, 8) ^ shr(x, 7);
}
}