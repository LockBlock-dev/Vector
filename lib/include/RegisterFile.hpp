#pragma once
#include "types.hpp"
#include <array>
#include "constants.hpp"

class RegisterFile
{
public:
    RegisterFile();

    WORD &operator[](BYTE reg);
    const WORD &operator[](BYTE reg) const;

    void reset();
    void state() const;

private:
    std::array<WORD, REGISTERS_COUNT> registers;
};
