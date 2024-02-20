#pragma once
#include "types.hpp"
#include "constants.hpp"
#include <vector>
#include <cmath>
#include <concepts>
#include <cstring>

constexpr int RAM_SIZE = 1024 * 1024;
constexpr int WORD_SIZE = sizeof(WORD);

// 2^XLEN
// constexpr uint64_t RAM_SIZE = 1ULL << XLEN;

class Memory {
public:
    void load(const std::vector<WORD>& program);

    template<std::integral T>
    T read(const WORD address) const
    {   
        constexpr unsigned int T_SIZE = sizeof(T);

        // ignores memory address overflow
        const WORD _address = address & ((1ULL << XLEN) - 1);

        T result = 0;

        // RISC-V is single byte-addressable
        // so we loop over each byte
        for (WORD i = 0; i < T_SIZE; ++i)
        {
            // calculates how much to shift each byte to position it correctly in the result
            // we subtract 1 to account for zero-based indexing, ensuring the least significant byte is shifted by 0 bits
            unsigned int shiftAmount = (T_SIZE - i - 1) * 8;

            result |= static_cast<T>(this->ram[_address + i] << shiftAmount);
        }

        return result;
    }

    template<std::integral T>
    void write(const WORD address, const T value)
    {
        constexpr unsigned int VAL_SIZE = sizeof(value);

        // ignores memory address overflow
        const WORD _address = address & ((1ULL << XLEN) - 1);

        // RISC-V is single byte-addressable
        // so we loop over each byte
        for (WORD i = 0; i < VAL_SIZE; ++i)
        {
            // calculates how much to shift each byte to position it correctly in the result
            // we subtract 1 to account for zero-based indexing, ensuring the least significant byte is shifted by 0 bits
            unsigned int shiftAmount = (VAL_SIZE - i - 1) * 8;

            this->ram[_address + i] = static_cast<BYTE>((value >> shiftAmount) & 0xFF);
        }
    }

    template<std::integral T>
    void copy_to(WORD address, WORD count, T* dst) const
    {
        // ignores memory address overflow
        const WORD _address = address & ((1ULL << XLEN) - 1);

        for (WORD i = 0; i < count; ++i) {
            dst[i] = this->read<T>(_address + i * sizeof(T));
        }
    }

private:
    BYTE ram[RAM_SIZE] {};
};
