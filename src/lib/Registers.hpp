#pragma once
#include <array>
#include "types.hpp"
#include "constants.hpp"

class Registers
{
public:
    WORD& get(const BYTE index);
    const WORD& get(const BYTE index) const;
    void state() const;
private:
    std::array<WORD, XLEN> registers {};
};
