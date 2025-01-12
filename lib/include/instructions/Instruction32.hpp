#pragma once
#include "../types.hpp"

class Instruction32
{
public:
    Instruction32(WORD raw);

    void decode();

    BYTE opcode;
    BYTE funct3;
    BYTE funct7;
    BYTE rd;
    BYTE rs1, rs2;
    WORD imm;

private:
    WORD raw;

    template <typename T>
    T decodeInstruction() const
    {
        return *reinterpret_cast<const T *>(&this->raw);
    }
};

struct RType
{
    WORD opcode : 7;
    WORD rd : 5;
    WORD funct3 : 3;
    WORD rs1 : 5;
    WORD rs2 : 5;
    WORD funct7 : 7;
};

struct IType
{
    WORD opcode : 7;
    WORD rd : 5;
    WORD funct3 : 3;
    WORD rs1 : 5;
    WORD imm : 12;
};

struct SType
{
    WORD opcode : 7;
    WORD imm1 : 5;
    WORD funct3 : 3;
    WORD rs1 : 5;
    WORD rs2 : 5;
    WORD imm2 : 7;
};

struct BType
{
    WORD opcode : 7;
    WORD imm1 : 1;
    WORD imm2 : 4;
    WORD funct3 : 3;
    WORD rs1 : 5;
    WORD rs2 : 5;
    WORD imm3 : 6;
    WORD imm4 : 1;
};

struct UType
{
    WORD opcode : 7;
    WORD rd : 5;
    WORD imm : 20;
};

struct JType
{
    WORD opcode : 7;
    WORD rd : 5;
    WORD imm1 : 8;
    WORD imm2 : 1;
    WORD imm3 : 10;
    WORD imm4 : 1;
};
