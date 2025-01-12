#pragma once
#include "types.hpp"
#include "Memory.hpp"
#include "instructions/Instruction32.hpp"
#include "RegisterFile.hpp"
#include "ExecutionUnit.hpp"
#include <array>
#include <chrono>

class Processor
{
public:
    Processor(Memory *memory);

    WORD fetch();
    Instruction32 decode(WORD raw);
    void step();

    WORD &reg(const size_t index);
    const WORD &reg(const size_t index) const;

    WORD &csr(const size_t csr);
    const WORD &csr(const size_t csr) const;

    WORD getPC() const;
    void incrementPC(WORD value);

    bool isStopped() const;
    void stop();

    void reset();
    void state() const;

private:
    WORD pc;

    RegisterFile registers;
    Memory *memory;
    ExecutionUnit exu;
    std::array<WORD, CSR_COUNT> CSRs;

    bool stopped;
    std::chrono::steady_clock::time_point startTime;
};
