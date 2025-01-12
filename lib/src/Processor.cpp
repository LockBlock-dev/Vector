#include "Processor.hpp"
#include <print>
#include <format>
#include <stdexcept>
#include <bitset>
#include "instruction_set.hpp"
#include <unordered_map>
#include <functional>

Processor::Processor(Memory *memory) : pc(0),
                                       registers(),
                                       memory(memory),
                                       exu(this, this->memory),
                                       CSRs({}),
                                       stopped(false),
                                       startTime(std::chrono::steady_clock::now())
{
}

WORD Processor::fetch()
{
    WORD instruction = 0;

    for (size_t i = 0; i < ILEN; i += PARCEL_SIZE)
    {
        HALFWORD parcel = this->memory->read<HALFWORD>(this->pc + i / 8);

        instruction |= static_cast<WORD>(parcel) << (PARCEL_SIZE - i);
    }

    return instruction;
}

Instruction32 Processor::decode(WORD raw)
{
    if (raw == 0)
        throw std::runtime_error("Illegal instruction exception.");

    if ((raw & 0b11) != 0b11)
        // 16 bits instruction
        throw std::runtime_error("16 bits instruction not implemented.");

    // 32 bits or more (we only support 32 bits)
    Instruction32 instruction{raw};

    instruction.decode();

    return instruction;
}

void Processor::step()
{
    auto instruction = this->fetch();

    this->exu.execute(this->decode(instruction));

    // Add the instruction size to the program counter
    this->pc += 4;

    WORD &cycle = this->CSRs.at(CSR_CYCLE);
    WORD &cycle_h = this->CSRs.at(CSR_CYCLEH);

    // Increment the cycle counter
    if (++cycle == 0) // If lower 32-bits overflows, increment the higher 32-bits
    {
        cycle_h++;

        if (cycle_h == 0) // If higher 32-bits overflows, reset the entire counter to zero
            cycle = 0;
    }

    WORD &time = this->CSRs.at(CSR_TIME);
    WORD &time_h = this->CSRs.at(CSR_TIMEH);

    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - this->startTime).count();

    time = static_cast<WORD>(elapsedTime);
    time_h = static_cast<WORD>(elapsedTime >> 32);

    WORD &instret = this->CSRs.at(CSR_INSTRET);
    WORD &instret_h = this->CSRs.at(CSR_INSTRETH);

    // Increment the retired instructions counter
    if (++instret == 0) // If lower 32-bits overflows, increment the higher 32-bits
    {
        instret_h++;

        if (instret_h == 0) // If higher 32-bits overflows, reset the entire counter to zero
            instret = 0;
    }
}

WORD &Processor::reg(const size_t index)
{
    if (index >= REGISTERS_COUNT)
        throw std::out_of_range("Invalid register.");

    return this->registers[index];
}

const WORD &Processor::reg(const size_t index) const
{
    if (index >= REGISTERS_COUNT)
        throw std::out_of_range("Invalid register.");

    return this->registers[index];
}

WORD &Processor::csr(const size_t csr)
{
    if (csr >= CSR_COUNT)
        throw std::out_of_range("Invalid CSR address.");

    if ((csr >= CSR_RO_BEGIN && csr <= CSR_RO_END) ||
        (csr >= CSR_H_RO_BEGIN && csr <= CSR_H_RO_END))
        throw std::runtime_error("CSR is read-only.");

    return this->CSRs.at(csr);
}

const WORD &Processor::csr(const size_t csr) const
{
    if (csr >= CSR_COUNT)
        throw std::out_of_range("Invalid CSR address.");

    return this->CSRs.at(csr);
}

WORD Processor::getPC() const
{
    return this->pc;
}

void Processor::incrementPC(WORD value)
{
    this->pc += value;
}

bool Processor::isStopped() const
{
    return this->stopped;
}

void Processor::stop()
{
    this->stopped = true;
}

void Processor::reset()
{
    this->pc = 0;
    this->stopped = false;
    this->CSRs.fill(0);

    this->registers.reset();
}

void Processor::state() const
{
    std::println("\t\t====== CPU STATE ======");
    std::println("pc = {}", this->pc);
    std::println("stopped = {}", this->stopped);
    std::println("");
    std::println("\t\t====== CPU CSRs ======");
    std::println("cycle = {}", (static_cast<DOUBLEWORD>(this->csr(CSR_CYCLEH)) << 32) | this->csr(CSR_CYCLE));
    std::println("time = {}", (static_cast<DOUBLEWORD>(this->csr(CSR_TIMEH)) << 32) | this->csr(CSR_TIME));
    std::println("instret = {}", (static_cast<DOUBLEWORD>(this->csr(CSR_INSTRETH)) << 32) | this->csr(CSR_INSTRET));
    std::println("");

    this->registers.state();
}
