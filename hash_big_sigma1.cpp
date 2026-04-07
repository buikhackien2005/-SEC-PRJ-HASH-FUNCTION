#include "hash_math_functions.h"

namespace sha512 {
// Big_Sigma_1 function used to rotate and mix bits
u64 BSIG1(u64 x) {
    return rotr(x, 14) ^ rotr(x, 18) ^ rotr(x, 41);
}
}