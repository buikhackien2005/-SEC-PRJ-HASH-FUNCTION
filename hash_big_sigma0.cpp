#include "hash_math_functions.h"

namespace sha512 {
// Big_Sigma_0 function used to rotate and mix bits
u64 BSIG0(u64 x) {
    return rotr(x, 28) ^ rotr(x, 34) ^ rotr(x, 39);
}
}