#include "Memory.hpp"
#include <stdexcept>

void Memory::load(const std::vector<WORD>& program)
{
    if (program.size() * WORD_SIZE >= RAM_SIZE)
        throw std::out_of_range("Memory capacity exceeded (program size: " + std::to_string(program.size() * WORD_SIZE) + ", max: " + std::to_string(RAM_SIZE) + ").");

    for (WORD i = 0; i < program.size(); i++) {
        this->write(i * (IALIGN / 8), program[i]);
    }
}
