#include "Registers.hpp"
#include <stdexcept>
#include <iostream>
#include <iomanip>

WORD& Registers::get(const BYTE index)
{
    if (index >= XLEN)
        throw std::out_of_range("Register index out of range (index: " + std::to_string(index) + ", max: " + std::to_string(XLEN - 1) + ").");
    
    if (index == 0) {
        // Discard writes to register 0
        static WORD dummy; // Create a dummy variable.
        return dummy; // Return a reference to the dummy variable.
    }

    return this->registers[index];
}

const WORD& Registers::get(const BYTE index) const
{
    if (index >= XLEN)
        throw std::out_of_range("Register index out of range (index: " + std::to_string(index) + ", max: " + std::to_string(XLEN - 1) + ").");

    return this->registers[index];
}

void Registers::state() const
{
    constexpr int numColumns = 4;
    // INT32_MAX width (10) + space
    constexpr int columnWidth = 11;
    constexpr int registerIndexWidth = 4;

    std::cout << "\t\t=== REGISTERS STATE ===" << "\n";

    for (WORD i = 0; i < this->registers.size(); ++i)
    {
        std::cout << std::left << std::setw(registerIndexWidth) << ("x" + std::to_string(i)) << "= " << std::setw(columnWidth) << static_cast<int32_t>(this->registers[i]);

        if ((i + 1) % numColumns == 0)
        {
            std::cout << "\n";
        }
    }

    std::cout << "\n";
}
