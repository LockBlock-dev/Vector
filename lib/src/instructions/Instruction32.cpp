#include "instructions/Instruction32.hpp"
#include "instruction_set.hpp"
#include <print>
#include <format>
#include <stdexcept>
#include <bitset>

Instruction32::Instruction32(WORD raw) : raw(raw),
                                         opcode(0),
                                         funct3(0),
                                         funct7(0),
                                         rd(0),
                                         rs1(0),
                                         rs2(0),
                                         imm(0)
{
}

void Instruction32::decode()
{
    this->opcode = (this->raw & 0b1111111);

    switch (this->opcode)
    {
    case RV32_OPCODE::RV32_LUI:
    {
        auto decoded = this->decodeInstruction<UType>();
        this->rd = decoded.rd;
        this->imm = decoded.imm << 12;

        break;
    }
    case RV32_OPCODE::RV32_AUIPC:
    {
        auto decoded = this->decodeInstruction<UType>();
        this->rd = decoded.rd;
        this->imm = decoded.imm << 12;

        break;
    }
    case RV32_OPCODE::RV32_JAL:
    {
        auto decoded = this->decodeInstruction<JType>();
        this->rd = decoded.rd;
        this->imm = (decoded.imm3 << 1) | (decoded.imm2 << 11) | (decoded.imm1 << 12) | (decoded.imm4 << 20);

        break;
    }
    case RV32_OPCODE::RV32_JALR:
    {
        auto decoded = this->decodeInstruction<IType>();
        this->rd = decoded.rd;
        this->rs1 = decoded.rs1;
        this->imm = decoded.imm;

        break;
    }
    case RV32_OPCODE::RV32_BRANCH:
    {
        auto decoded = this->decodeInstruction<BType>();
        this->imm = (decoded.imm2 << 1) | (decoded.imm3 << 5) | (decoded.imm1 << 11) | (decoded.imm4 << 12);
        this->rs1 = decoded.rs1;
        this->rs2 = decoded.rs2;
        this->funct3 = decoded.funct3;

        break;
    }
    case RV32_OPCODE::RV32_LOAD:
    {
        auto decoded = this->decodeInstruction<IType>();
        this->rd = decoded.rd;
        this->rs1 = decoded.rs1;
        this->imm = decoded.imm;
        this->funct3 = decoded.funct3;

        break;
    }
    case RV32_OPCODE::RV32_STORE:
    {
        auto decoded = this->decodeInstruction<SType>();
        this->rs1 = decoded.rs1;
        this->rs2 = decoded.rs2;
        this->imm = decoded.imm1 | (decoded.imm2 << 5);
        this->funct3 = decoded.funct3;

        break;
    }
    case RV32_OPCODE::RV32_OP_IMM:
    {
        auto decoded = this->decodeInstruction<IType>();
        this->rd = decoded.rd;
        this->rs1 = decoded.rs1;
        this->imm = decoded.imm;
        this->funct3 = decoded.funct3;
        this->funct7 = (this->imm >> 5) & 0b1111111;

        break;
    }
    case RV32_OPCODE::RV32_OP:
    {
        auto decoded = this->decodeInstruction<RType>();
        this->rd = decoded.rd;
        this->rs1 = decoded.rs1;
        this->rs2 = decoded.rs2;
        this->funct3 = decoded.funct3;
        this->funct7 = decoded.funct7;

        break;
    }
    case RV32_OPCODE::RV32_MISC_MEM:
    {
        auto decoded = this->decodeInstruction<IType>();
        this->rd = decoded.rd;
        this->rs1 = decoded.rs1;
        this->imm = decoded.imm;

        break;
    }
    case RV32_OPCODE::RV32_SYSTEM:
    {
        auto decoded = this->decodeInstruction<IType>();

        this->rd = decoded.rd;
        this->rs1 = decoded.rs1;
        this->imm = decoded.imm; // funct12
        this->funct3 = decoded.funct3;

        break;
    }
    default:
        throw std::runtime_error(std::format("Unhandled opcode: 0b{}", std::bitset<7>(opcode).to_string()));
    };
}
