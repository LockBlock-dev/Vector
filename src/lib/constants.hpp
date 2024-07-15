#pragma once
#include "types.hpp"

constexpr unsigned int XLEN = 32; // 32 (RV32I) / 64 (RV64I) / 128 (RV128I)
constexpr unsigned int IALIGN = 32; // 32 (RVI) / 16 (RVC)

constexpr unsigned int SYSCALL_WRITE = 64;
constexpr unsigned int SYSCALL_EXIT  = 93;

constexpr unsigned int RV32I_LOAD   = 0b0000011;
constexpr unsigned int RV32I_OP_IMM = 0b0010011;
constexpr unsigned int RV32I_AUIPC  = 0b0010111;
constexpr unsigned int RV32I_STORE  = 0b0100011;
constexpr unsigned int RV32I_OP     = 0b0110011;
constexpr unsigned int RV32I_LUI    = 0b0110111;
constexpr unsigned int RV32I_BRANCH = 0b1100011;
constexpr unsigned int RV32I_JALR   = 0b1100111;
constexpr unsigned int RV32I_JAL    = 0b1101111;
constexpr unsigned int RV32I_SYSTEM = 0b1110011;

constexpr unsigned int REG_ZERO  = 0;
constexpr unsigned int REG_RA    = 1;
constexpr unsigned int REG_SP    = 2;
constexpr unsigned int REG_GP    = 3;
constexpr unsigned int REG_TP    = 4;
constexpr unsigned int REG_T0    = 5;
constexpr unsigned int REG_T1    = 6;
constexpr unsigned int REG_T2    = 7;
constexpr unsigned int REG_S0    = 8;
constexpr unsigned int REG_FP    = 8;
constexpr unsigned int REG_S1    = 9;
constexpr unsigned int REG_A0    = 10;
constexpr unsigned int REG_RET0  = 10;
constexpr unsigned int REG_A1    = 11;
constexpr unsigned int REG_RET1  = 11;
constexpr unsigned int REG_A2    = 12;
constexpr unsigned int REG_A3    = 13;
constexpr unsigned int REG_A4    = 14;
constexpr unsigned int REG_A5    = 15;
constexpr unsigned int REG_A6    = 16;
constexpr unsigned int REG_A7    = 17;
constexpr unsigned int REG_ECALL = 17;
constexpr unsigned int REG_S2    = 18;
constexpr unsigned int REG_S3    = 19;
constexpr unsigned int REG_S4    = 20;
constexpr unsigned int REG_S5    = 21;
constexpr unsigned int REG_S6    = 22;
constexpr unsigned int REG_S7    = 23;
constexpr unsigned int REG_S8    = 24;
constexpr unsigned int REG_S9    = 25;
constexpr unsigned int REG_S10   = 26;
constexpr unsigned int REG_S11   = 27;
constexpr unsigned int REG_T3    = 28;
constexpr unsigned int REG_T4    = 29;
constexpr unsigned int REG_T5    = 30;
constexpr unsigned int REG_T6    = 31;

struct RType {
    WORD opcode : 7;
    WORD rd     : 5;
    WORD funct3 : 3;
    WORD rs1    : 5;
    WORD rs2    : 5;
    WORD funct7 : 7;
};

struct IType {
    WORD opcode : 7;
    WORD rd     : 5;
    WORD funct3 : 3;
    WORD rs1    : 5;
    WORD imm    : 12;
};

struct SType {
    WORD opcode : 7;
    WORD imm1   : 5;
    WORD funct3 : 3;
    WORD rs1    : 5;
    WORD rs2    : 5;
    WORD imm2   : 7;
};

struct BType {
    WORD opcode : 7;
    WORD imm1   : 1;
    WORD imm2   : 4;
    WORD funct3 : 3;
    WORD rs1    : 5;
    WORD rs2    : 5;
    WORD imm3   : 6;
    WORD imm4   : 1;
};

struct UType {
    WORD opcode : 7;
    WORD rd     : 5;
    WORD imm    : 20;
};

struct JType {
    WORD opcode : 7;
    WORD rd     : 5;
    WORD imm1   : 8;
    WORD imm2   : 1;
    WORD imm3   : 10;
    WORD imm4   : 1;
};
