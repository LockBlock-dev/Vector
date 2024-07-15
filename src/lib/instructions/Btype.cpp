#include "../Processor.hpp"
#include "../utils.hpp"
#include <stdexcept>
#include <format>

void Processor::BEQ()
{
    if (this->Register(this->rs1) == this->Register(this->rs2))
    {
        WORD offset = signextend(this->imm);

        if (offset % 4 != 0)
            throw std::runtime_error("Instruction address misaligned (offset: 0x" + std::format("{:X}", offset) + ").");

        // PC is incremented post instruction execution
        this->pc += offset - 4;
    }
}

void Processor::BNE()
{
    if (this->Register(this->rs1) != this->Register(this->rs2))
    {
        WORD offset = signextend(this->imm);

        if (offset % 4 != 0)
            throw std::runtime_error("Instruction address misaligned (offset: 0x" + std::format("{:X}", offset) + ").");


        // PC is incremented post instruction execution
        this->pc += offset - 4;
    }
}

void Processor::BLT()
{
    if (static_cast<int32_t>(this->Register(this->rs1)) < static_cast<int32_t>(this->Register(this->rs2)))
    {
        WORD offset = signextend(this->imm);

        if (offset % 4 != 0)
            throw std::runtime_error("Instruction address misaligned (offset: 0x" + std::format("{:X}", offset) + ").");

        // PC is incremented post instruction execution
        this->pc += offset - 4;
    }
}

void Processor::BGE()
{
    if (static_cast<int32_t>(this->Register(this->rs1)) >= static_cast<int32_t>(this->Register(this->rs2)))
    {
        WORD offset = signextend(this->imm);

        if (offset % 4 != 0)
            throw std::runtime_error("Instruction address misaligned (offset: 0x" + std::format("{:X}", offset) + ").");

        // PC is incremented post instruction execution
        this->pc += offset - 4;
    }
}

void Processor::BLTU()
{
    if (this->Register(this->rs1) < this->Register(this->rs2))
    {
        WORD offset = signextend(this->imm);

        if (offset % 4 != 0)
            throw std::runtime_error("Instruction address misaligned (offset: 0x" + std::format("{:X}", offset) + ").");

        // PC is incremented post instruction execution
        this->pc += offset - 4;
    }
}

void Processor::BGEU()
{
    if (this->Register(this->rs1) >= this->Register(this->rs2))
    {
        WORD offset = signextend(this->imm);

        if (offset % 4 != 0)
            throw std::runtime_error("Instruction address misaligned (offset: 0x" + std::format("{:X}", offset) + ").");

        // PC is incremented post instruction execution
        this->pc += offset - 4;
    }
}
