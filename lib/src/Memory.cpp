#include "Memory.hpp"
#include <algorithm>

Memory::Memory(size_t size) : memory(size, 0) {}

void Memory::reset()
{
    std::fill(this->memory.begin(), this->memory.end(), 0);
}
