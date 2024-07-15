#pragma once
#include "types.hpp"

/*
https://graphics.stanford.edu/~seander/bithacks.html#VariableSignExtend
*/
int32_t signextend(WORD x, int bitWidth = 12 /*most immediates are 12 bits long*/);
