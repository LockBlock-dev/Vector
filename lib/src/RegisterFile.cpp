#include "RegisterFile.hpp"
#include <stdexcept>
#include <format>
#include <print>

RegisterFile::RegisterFile() : registers({})
{
    this->reset();
}

WORD &RegisterFile::operator[](BYTE reg)
{
    if (reg >= REGISTERS_COUNT)
        throw std::out_of_range(std::format("Register index out of range (index: {}, max: {}).", reg, REGISTERS_COUNT - 1));

    if (reg == 0)
    {
        static WORD zero = 0;
        return zero;
    }

    return this->registers.at(reg);
}

const WORD &RegisterFile::operator[](BYTE reg) const
{
    if (reg >= REGISTERS_COUNT)
        throw std::out_of_range(std::format("Register index out of range (index: {}, max: {}).", reg, REGISTERS_COUNT - 1));

    if (reg == 0)
    {
        static const WORD zero = 0;
        return zero;
    }

    return this->registers.at(reg);
}

void RegisterFile::reset()
{
    this->registers.fill(0);
}

void RegisterFile::state() const
{
    constexpr int numColumns = 4;
    constexpr int columnWidth = 11; // INT32_MAX width (10) + space
    constexpr int registerIndexWidth = 4;

    std::println("\t\t=== REGISTERS STATE ===");

    for (size_t i = 0; i < this->registers.size(); ++i)
    {
        std::print("x{:<{}}= {:<{}}", i, registerIndexWidth, static_cast<int32_t>(this->registers[i]), columnWidth);

        if ((i + 1) % numColumns == 0)
            std::println("");
    }

    if (this->registers.size() % numColumns != 0)
        std::println("");
}
