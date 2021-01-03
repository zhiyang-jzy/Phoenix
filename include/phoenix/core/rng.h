

#pragma once
#include "phoenix.h"

PHOENIX_NAMESPACE_BEGIN

#define PCG32_DEFAULT_STATE 0x853c49e6748fea9bULL
#define PCG32_DEFAULT_STREAM 0xda3e39cb94b95bdbULL
#define PCG32_MULT 0x5851f42d4c957f2dULL

class RNG{
private:
    uint64_t state,inc;
public:
    RNG():state(PCG32_DEFAULT_STATE),inc(PCG32_DEFAULT_STREAM){}

    uint32_t nextInt();



};


PHOENIX_NAMESPACE_END

