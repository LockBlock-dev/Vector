#include "utils.hpp"
#include "constants.hpp"
#include <climits>

int32_t signextend(WORD x, int bitWidth)
{
    const int m = sizeof(x) * CHAR_BIT - bitWidth;
    return (x << m) >> m;
}
