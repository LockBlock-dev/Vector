#include "../Processor.hpp"
#include "../utils.hpp"
#include <stdexcept>
#include <format>
#include <cstdio>
#include <unistd.h>

void Processor::ADDI()
{
    WORD& dst = this->Register(this->rd);

    WORD result = this->Register(this->rs1) + signextend(this->imm);

    dst = result & ((1ULL << XLEN) - 1);
}

void Processor::SLTI()
{
    WORD& dst = this->Register(this->rd);

    dst = static_cast<int32_t>(this->Register(this->rs1)) < signextend(this->imm);
}

void Processor::SLTIU()
{
    WORD& dst = this->Register(this->rd);

    dst = this->Register(this->rs1) < this->imm;
}

void Processor::XORI()
{
    WORD& dst = this->Register(this->rd);

    dst = this->Register(this->rs1) ^ signextend(this->imm);
}

void Processor::ORI()
{
    WORD& dst = this->Register(this->rd);

    dst = this->Register(this->rs1) | signextend(this->imm);
}

void Processor::ANDI()
{
    WORD& dst = this->Register(this->rd);

    dst = this->Register(this->rs1) & signextend(this->imm);
}

void Processor::SLLI()
{
    WORD& dst = this->Register(this->rd);

    WORD shamt = (this->imm & 0b11111);

    dst = this->Register(this->rs1) << shamt;
}

void Processor::SRLI()
{
    WORD& dst = this->Register(this->rd);

    WORD shamt = (this->imm & 0b11111);

    dst = this->Register(this->rs1) >> shamt;
}

void Processor::SRAI()
{
    WORD& dst = this->Register(this->rd);

    WORD shamt = (this->imm & 0b11111);

    dst = static_cast<int32_t>(this->Register(this->rs1)) >> shamt;
}



void Processor::LB()
{
    WORD& dst = this->Register(this->rd);

    WORD address = this->Register(this->rs1) + signextend(this->imm);

    dst = signextend(this->memory.read<BYTE>(address), sizeof(BYTE) * 8);
}

void Processor::LH()
{
    WORD& dst = this->Register(this->rd);

    WORD address = this->Register(this->rs1) + signextend(this->imm);

    dst = signextend(this->memory.read<HALFWORD>(address), sizeof(HALFWORD) * 8);
}

void Processor::LW()
{
    WORD& dst = this->Register(this->rd);

    WORD address = this->Register(this->rs1) + signextend(this->imm);

    dst = this->memory.read<WORD>(address);
}

void Processor::LBU()
{
    WORD& dst = this->Register(this->rd);

    WORD address = this->Register(this->rs1) + signextend(this->imm);

    dst = this->memory.read<BYTE>(address);
}

void Processor::LHU()
{
    WORD& dst = this->Register(this->rd);

    WORD address = this->Register(this->rs1) + signextend(this->imm);

    dst = this->memory.read<HALFWORD>(address);
}



void Processor::JALR()
{
    WORD& dst = this->Register(this->rd);

    WORD address = (this->Register(this->rs1) + signextend(this->imm)) & ~1;

    if (address % 4 != 0)
        throw std::runtime_error("Instruction address misaligned (offset: 0x" + std::format("{:X}", address) + ").");

    dst = this->pc + 4;

    // PC is incremented post instruction execution
    this->pc += address - 4;
}



void Processor::ECALL()
{
    WORD syscall = this->Register(REG_ECALL);

    switch (syscall)
    {
        case SYSCALL_WRITE: {
            int32_t fd   = this->Register(REG_A0);
            WORD address = this->Register(REG_A1);
            WORD len     = this->Register(REG_A2);

            if (fd == fileno(stdout) || fd == fileno(stderr))
            {
                WORD max = 128;
                WORD count = std::min(max, len);
                std::vector<BYTE> buffer(count);

                this->memory.copy_to<BYTE>(address, count, buffer.data());

                this->Register(REG_RET0) = static_cast<WORD>(write(fd, buffer.data(), count));
            }

            break;
        }

        case SYSCALL_EXIT: {
            int32_t code = static_cast<int32_t>(this->Register(REG_A0));

            exit(code);
        }
        
        default:
            throw std::runtime_error("Unhandled syscall (code: " + std::format("{}", syscall) + ").");
    }
}

void Processor::EBREAK()
{
    throw std::runtime_error("Unhandled syscall (EBREAK).");
}
