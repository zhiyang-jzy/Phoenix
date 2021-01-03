//
// Created by jzy99 on 2021/1/2.
//

#include <phoenix/core/rng.h>

PHOENIX_NAMESPACE_BEGIN


uint32_t RNG::nextInt() {
    uint64_t oldstate = state;
    // Advance internal state
    state = oldstate * 6364136223846793005ULL + (inc|1);
    // Calculate output function (XSH RR), uses old state for max ILP
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}


PHOENIX_NAMESPACE_END

