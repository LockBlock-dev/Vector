#include "ExecutionUnit.hpp"
#include "Processor.hpp"
#include "Memory.hpp"
#include "instruction_set.hpp"
#include "utils.hpp"
#include <stdexcept>
#include <format>
#include <bitset>
#include <unistd.h>

ExecutionUnit::ExecutionUnit(Processor *processor, Memory *memory) : proc(processor), mem(memory), instr(0) {}

void ExecutionUnit::execute(const Instruction32 &instruction)
{
    this->instr = instruction;

    switch (this->instr.opcode)
    {
    case RV32_OPCODE::RV32_LUI:
    {
        this->LUI();
        break;
    }
    case RV32_OPCODE::RV32_AUIPC:
    {
        this->AUIPC();
        break;
    }
    case RV32_OPCODE::RV32_JAL:
    {
        this->JAL();
        break;
    }
    case RV32_OPCODE::RV32_JALR:
    {
        this->JALR();
        break;
    }
    case RV32_OPCODE::RV32_BRANCH:
    {
        this->BRANCH();
        break;
    }
    case RV32_OPCODE::RV32_LOAD:
    {
        this->LOAD();
        break;
    }
    case RV32_OPCODE::RV32_STORE:
    {
        this->STORE();
        break;
    }
    case RV32_OPCODE::RV32_OP_IMM:
    {
        this->OP_IMM();
        break;
    }
    case RV32_OPCODE::RV32_OP:
    {
        this->OP();
        break;
    }
    case RV32_OPCODE::RV32_MISC_MEM:
    {
        this->MISC_MEM();
        break;
    }
    case RV32_OPCODE::RV32_SYSTEM:
    {
        this->SYSTEM();
        break;
    }

    default:
        throw std::runtime_error(std::format("Unhandled opcode: 0b{}.", std::bitset<7>(this->instr.opcode).to_string()));
    }
}

void ExecutionUnit::OP()
{
    WORD &rd = this->proc->reg(this->instr.rd);
    WORD rs1 = this->proc->reg(this->instr.rs1);
    WORD rs2 = this->proc->reg(this->instr.rs2);

    switch (this->instr.funct7)
    {
    case RV32_FUNCT7::RV32I_FUNCT7:
    {
        switch (this->instr.funct3)
        {
        case RV32_FUNCT3::RV32I_FUNCT3_SLL:
        {
            rd = rs1 << rs2;
            break;
        }
        case RV32_FUNCT3::RV32I_FUNCT3_SLT:
        {
            rd = static_cast<int32_t>(rs1) < static_cast<int32_t>(rs2);
            break;
        }
        case RV32_FUNCT3::RV32I_FUNCT3_SLTU:
        {
            rd = rs1 < rs2;
            break;
        }
        case RV32_FUNCT3::RV32I_FUNCT3_XOR:
        {
            rd = rs1 ^ rs2;
            break;
        }
        case RV32_FUNCT3::RV32I_FUNCT3_OR:
        {
            rd = rs1 | rs2;
            break;
        }
        case RV32_FUNCT3::RV32I_FUNCT3_AND:
        {
            rd = rs1 & rs2;
            break;
        }
        case RV32_FUNCT3::RV32I_FUNCT3_ADD:
        {
            rd = (rs1 + rs2) & XLEN_MASK;
            break;
        }
        case RV32_FUNCT3::RV32I_FUNCT3_SRL:
        {
            rd = rs1 >> rs2;
            break;
        }
        default:
            throw std::runtime_error(std::format("Unhandled RV32_OP funct3 (funct3: 0b{}).", std::bitset<3>(this->instr.funct3).to_string()));
        }

        break;
    }
    case RV32_FUNCT7::RV32I_FUNCT7_ALT:
    {
        switch (this->instr.funct3)
        {
        case RV32_FUNCT3::RV32I_FUNCT3_SUB:
        {
            rd = (rs1 - rs2) & XLEN_MASK;
            break;
        }
        case RV32_FUNCT3::RV32I_FUNCT3_SRA:
        {
            rd = static_cast<int32_t>(rs1) >> static_cast<int32_t>(rs2);
            break;
        }
        default:
            throw std::runtime_error(std::format("Unhandled RV32_OP funct3 (funct3: 0b{}).", std::bitset<3>(this->instr.funct3).to_string()));
        }

        break;
    }
    case RV32_FUNCT7::RV32M_FUNCT7:
    {
        switch (this->instr.funct3)
        {
        case RV32_FUNCT3::RV32M_FUNCT3_MUL:
        {
            rd = static_cast<WORD>((static_cast<DOUBLEWORD>(rs1) * static_cast<DOUBLEWORD>(rs2)) & XLEN_MASK);
            break;
        }
        case RV32_FUNCT3::RV32M_FUNCT3_MULH:
        {
            rd = static_cast<int32_t>((static_cast<int64_t>(rs1) * static_cast<int64_t>(rs2)) >> XLEN);
            break;
        }
        case RV32_FUNCT3::RV32M_FUNCT3_MULHSU:
        {
            rd = static_cast<int32_t>(static_cast<int64_t>(rs1) * static_cast<DOUBLEWORD>(rs2) >> XLEN);
            break;
        }
        case RV32_FUNCT3::RV32M_FUNCT3_MULHU:
        {
            rd = static_cast<WORD>((static_cast<DOUBLEWORD>(rs1) * static_cast<DOUBLEWORD>(rs2)) >> XLEN);
            break;
        }
        case RV32_FUNCT3::RV32M_FUNCT3_DIV:
        {
            if (rs2 == 0)
                rd = XLEN_MASK;
            else if (static_cast<int32_t>(rs1) == SIGNED_MIN && static_cast<int32_t>(rs2) == -1)
                rd = static_cast<int32_t>(rs1);
            else
                rd = static_cast<int32_t>(static_cast<int32_t>(rs1) / static_cast<int32_t>(rs2));
            break;
        }
        case RV32_FUNCT3::RV32M_FUNCT3_DIVU:
        {

            if (rs2 == 0)
                rd = XLEN_MASK;
            else
                rd = rs1 / rs2;
            break;
        }
        case RV32_FUNCT3::RV32M_FUNCT3_REM:
        {
            if (rs2 == 0)
                rd = static_cast<int32_t>(rs1);
            else if (static_cast<int32_t>(rs1) == SIGNED_MIN && static_cast<int32_t>(rs2) == -1)
                rd = 0;
            else
                rd = static_cast<int32_t>(rs1) % static_cast<int32_t>(rs2);
            break;
        }
        case RV32_FUNCT3::RV32M_FUNCT3_REMU:
        {
            if (rs2 == 0)
                rd = rs1;
            else
                rd = rs1 % rs2;
            break;
        }
        default:
            throw std::runtime_error(std::format("Unhandled RV32M_OP funct3 (funct3: 0b{}).", std::bitset<3>(this->instr.funct3).to_string()));
        }

        break;
    }
    default:
        throw std::runtime_error(std::format("Unhandled RV32_OP funct7 (funct7: 0b{}).", std::bitset<7>(this->instr.funct7).to_string()));
    }
}

void ExecutionUnit::OP_IMM()
{
    WORD &rd = this->proc->reg(this->instr.rd);
    WORD rs1 = this->proc->reg(this->instr.rs1);

    switch (this->instr.funct3)
    {
    case RV32_FUNCT3::RV32I_FUNCT3_ADDI:
    {
        rd = (rs1 + signextend(this->instr.imm, 12)) & XLEN_MASK;
        break;
    }

    case RV32_FUNCT3::RV32I_FUNCT3_SLLI:
    {
        WORD shamt = (this->instr.imm & 0b11111);
        rd = rs1 << shamt;
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_SLTI:
    {
        rd = static_cast<int32_t>(rs1) < signextend(this->instr.imm, 12);
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_SLTIU:
    {
        rd = rs1 < this->instr.imm;
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_XORI:
    {
        rd = rs1 ^ signextend(this->instr.imm, 12);
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_ORI:
    {
        rd = rs1 | signextend(this->instr.imm, 12);
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_ANDI:
    {
        rd = rs1 & signextend(this->instr.imm, 12);
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_SRLI_SRAI:
    {
        switch (this->instr.funct7)
        {
        case RV32_FUNCT7::RV32I_FUNCT7_SRLI:
        {
            WORD shamt = (this->instr.imm & 0b11111);
            rd = rs1 >> shamt;
            break;
        }
        case RV32_FUNCT7::RV32I_FUNCT7_SRAI:
        {
            WORD shamt = (this->instr.imm & 0b11111);
            rd = static_cast<int32_t>(rs1) >> shamt;
            break;
        }
        default:
            throw std::runtime_error(std::format("Unhandled RV32_OP funct7 for SRLI_SRAI (funct7: 0b{}).", std::bitset<7>(this->instr.funct7).to_string()));
        }

        break;
    }
    default:
        throw std::runtime_error(std::format("Unhandled RV32_OP_IMM funct3 (funct3: 0b{}).", std::bitset<3>(this->instr.funct3).to_string()));
    }
}

void ExecutionUnit::LOAD()
{
    WORD &rd = this->proc->reg(this->instr.rd);
    WORD rs1 = this->proc->reg(this->instr.rs1);
    WORD address = rs1 + signextend(this->instr.imm, 12);

    switch (this->instr.funct3)
    {
    case RV32_FUNCT3::RV32I_FUNCT3_LB:
    {
        rd = signextend(this->mem->read<BYTE>(address), sizeof(BYTE) * 8);
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_LH:
    {
        rd = signextend(this->mem->read<HALFWORD>(address), sizeof(HALFWORD) * 8);
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_LW:
    {
        rd = this->mem->read<WORD>(address);
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_LBU:
    {
        rd = this->mem->read<BYTE>(address);
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_LHU:
    {
        rd = this->mem->read<HALFWORD>(address);
        break;
    }
    default:
        throw std::runtime_error(std::format("Unhandled RV32_LOAD funct3 (funct3: 0b{}).", std::bitset<3>(this->instr.funct3).to_string()));
    }
}

void ExecutionUnit::JALR()
{
    WORD address = (this->proc->reg(this->instr.rs1) + signextend(this->instr.imm, 12)) & ~1;

    if (address % 4 != 0)
        throw std::runtime_error(std::format("Instruction address misaligned (offset: 0x{:X}).", address));

    this->proc->reg(this->instr.rd) = this->proc->getPC() + 4;

    this->proc->incrementPC(address - 4);
}

void ExecutionUnit::MISC_MEM()
{
    WORD succ = this->instr.imm & 0b1111;
    WORD pred = (this->instr.imm >> 4) & 0b1111;
    WORD fm = (this->instr.imm >> 8) & 0b1111;

    switch (fm)
    {
    case RV32I_FENCE::RV32I_FENCE_NORMAL:
        break;

    case RV32I_FENCE::RV32I_FENCE_TSO:
        break;
    }
}

void ExecutionUnit::SYSTEM()
{
    WORD &rd = this->proc->reg(this->instr.rd);
    WORD rs1 = this->proc->reg(this->instr.rs1);
    WORD imm = this->instr.imm;

    switch (this->instr.funct3)
    {
    case RV32I_FUNCT3_PRIV:
    {
        // funct12
        switch (imm)
        {
        case RV32I_FUNCT12::RV32I_FUNCT12_ECALL:
        {
            WORD syscall = this->proc->reg(REG_ECALL);

            switch (syscall)
            {
            case SYSCALL_WRITE:
            {
                int32_t fd = this->proc->reg(REG_A0);
                WORD address = this->proc->reg(REG_A1);
                WORD len = this->proc->reg(REG_A2);

                if (fd == fileno(stdout) || fd == fileno(stderr))
                {
                    WORD max = 128;
                    WORD count = std::min(max, len);
                    std::vector<BYTE> buffer(count);

                    this->mem->copy_to<BYTE>(address, count, buffer.data());

                    this->proc->reg(REG_RET0) = static_cast<WORD>(write(fd, buffer.data(), count));
                }

                break;
            }
            case SYSCALL_EXIT:
            {
                int32_t code = static_cast<int32_t>(this->proc->reg(REG_A0));

                this->proc->stop();

                if (code != EXIT_SUCCESS)
                    throw std::runtime_error(std::format("Emulator stopped with exit code {}", code));

                // exit(code);

                break;
            }
            default:
                throw std::runtime_error(std::format("Unhandled syscall (code: {}).", syscall));
            }

            break;
        }
        case RV32I_FUNCT12::RV32I_FUNCT12_EBREAK:
        {
            throw std::runtime_error("Unhandled syscall (EBREAK).");
            break;
        }
        default:
            throw std::runtime_error(std::format("Unhandled RV32_SYSTEM funct12 (funct12: 0b{}).", std::bitset<12>(this->instr.imm).to_string()));
        }

        break;
    }
    case RV32Zicsr_FUNCT3_CSRRW:
    {
        if (this->instr.rd != REG_ZERO)
            rd = this->proc->csr(imm);

        this->proc->csr(imm) = rs1;
        break;
    }
    case RV32Zicsr_FUNCT3_CSRRS:
    {
        rd = this->proc->csr(imm);

        if (this->instr.rs1 != REG_ZERO)
            this->proc->csr(imm) |= rs1;
        break;
    }
    case RV32Zicsr_FUNCT3_CSRRC:
    {
        rd = this->proc->csr(imm);

        if (this->instr.rs1 != REG_ZERO)
            this->proc->csr(imm) &= ~rs1;
        break;
    }
    case RV32Zicsr_FUNCT3_CSRRWI:
    {
        if (this->instr.rd != REG_ZERO)
            rd = this->proc->csr(imm);

        this->proc->csr(imm) = static_cast<WORD>(this->instr.rs1);
        break;
    }
    case RV32Zicsr_FUNCT3_CSRRSI:
    {
        rd = this->proc->csr(imm);

        if (this->instr.rs1 != 0)
            this->proc->csr(imm) |= static_cast<WORD>(this->instr.rs1);
        break;
    }
    case RV32Zicsr_FUNCT3_CSRRCI:
    {
        rd = this->proc->csr(imm);

        if (this->instr.rs1 != 0)
            this->proc->csr(imm) &= ~static_cast<WORD>(this->instr.rs1);
        break;
    }
    default:
        throw std::runtime_error(std::format("Unhandled RV32_SYSTEM funct3 (funct3: 0b{}).", std::bitset<3>(this->instr.funct3).to_string()));
    }
}

void ExecutionUnit::STORE()
{
    WORD rs1 = this->proc->reg(this->instr.rs1);
    WORD rs2 = this->proc->reg(this->instr.rs2);

    switch (this->instr.funct3)
    {
    case RV32_FUNCT3::RV32I_FUNCT3_SB:
    {
        WORD address = rs1 + signextend(this->instr.imm, 12);
        this->mem->write<BYTE>(address, static_cast<BYTE>(rs2));
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_SH:
    {
        WORD address = rs1 + signextend(this->instr.imm, 12);
        this->mem->write<HALFWORD>(address, static_cast<HALFWORD>(rs2));
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_SW:
    {
        WORD address = rs1 + signextend(this->instr.imm, 12);
        this->mem->write<WORD>(address, static_cast<WORD>(rs2));
        break;
    }
    default:
        throw std::runtime_error(std::format("Unhandled RV32_STORE funct3 (funct3: 0b{}).", std::bitset<3>(this->instr.funct3).to_string()));
    }
}

void ExecutionUnit::BRANCH()
{
    bool branch = false;
    WORD offset = signextend(this->instr.imm, 12);

    if (offset % 4 != 0)
        throw std::runtime_error(std::format("Instruction address misaligned (offset: 0x{:X}).", offset));

    WORD rs1 = this->proc->reg(this->instr.rs1);
    WORD rs2 = this->proc->reg(this->instr.rs2);

    switch (this->instr.funct3)
    {
    case RV32_FUNCT3::RV32I_FUNCT3_BEQ:
    {
        branch = rs1 == rs2;
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_BNE:
    {
        branch = rs1 != rs2;
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_BLT:
    {
        branch = static_cast<int32_t>(rs1) < static_cast<int32_t>(rs2);
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_BGE:
    {
        branch = static_cast<int32_t>(rs1) >= static_cast<int32_t>(rs2);
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_BLTU:
    {
        branch = rs1 < rs2;
        break;
    }
    case RV32_FUNCT3::RV32I_FUNCT3_BGEU:
    {
        branch = rs1 >= rs2;
        break;
    }
    default:
        throw std::runtime_error(std::format("Unhandled RV32_BRANCH funct3 (funct3: 0b{}).", std::bitset<3>(this->instr.funct3).to_string()));
    }

    if (branch)
        this->proc->incrementPC(offset - 4);
}

void ExecutionUnit::LUI()
{
    this->proc->reg(this->instr.rd) = signextend(this->instr.imm, 20);
}

void ExecutionUnit::AUIPC()
{
    this->proc->reg(this->instr.rd) = this->proc->getPC() + signextend(this->instr.imm, 20);
}

void ExecutionUnit::JAL()
{
    WORD offset = signextend(this->instr.imm, 20);

    if (offset % 4 != 0)
        throw std::runtime_error(std::format("Instruction address misaligned (offset: 0x{:X}).", offset));

    this->proc->reg(this->instr.rd) = this->proc->getPC() + 4;

    this->proc->incrementPC(offset - 4);
}
