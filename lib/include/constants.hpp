#pragma once
#include "types.hpp"

/*
Width of an integer register.

- 32  : RV32I
- 64  : RV64I
- 128 : RV128I
*/
constexpr unsigned int XLEN = 32;

/*
Instruction-address alignment constraint.

- 32 : RVI
- 16 : RVC
*/
constexpr unsigned int IALIGN = 32;

/*
Maximum instruction length supported.
Always a multiple of `IALIGN`.

- 32 : RV32I
*/
constexpr unsigned int ILEN = 32;

/*
Number of registers.

- 32 : RV32I
- 16 : RV32E / RV64E
*/
constexpr unsigned int REGISTERS_COUNT = 32;

constexpr unsigned int PARCEL_SIZE = 16;

// 2^XLEN
// constexpr DOUBLEWORD RAM_SIZE = 1ULL << XLEN;
constexpr unsigned int RAM_SIZE = 1024 * 1024;

constexpr unsigned int CSR_COUNT = 4096;

constexpr DOUBLEWORD XLEN_MASK = ((1ULL << XLEN) - 1);

constexpr int64_t SIGNED_MIN = -(1LL << (XLEN - 1));

constexpr unsigned int SYSCALL_WRITE = 64;
constexpr unsigned int SYSCALL_EXIT = 93;

constexpr unsigned int REG_ZERO = 0;
constexpr unsigned int REG_RA = 1;
constexpr unsigned int REG_SP = 2;
constexpr unsigned int REG_GP = 3;
constexpr unsigned int REG_TP = 4;
constexpr unsigned int REG_T0 = 5;
constexpr unsigned int REG_T1 = 6;
constexpr unsigned int REG_T2 = 7;
constexpr unsigned int REG_S0 = 8;
constexpr unsigned int REG_FP = 8;
constexpr unsigned int REG_S1 = 9;
constexpr unsigned int REG_A0 = 10;
constexpr unsigned int REG_RET0 = 10;
constexpr unsigned int REG_A1 = 11;
constexpr unsigned int REG_RET1 = 11;
constexpr unsigned int REG_A2 = 12;
constexpr unsigned int REG_A3 = 13;
constexpr unsigned int REG_A4 = 14;
constexpr unsigned int REG_A5 = 15;
constexpr unsigned int REG_A6 = 16;
constexpr unsigned int REG_A7 = 17;
constexpr unsigned int REG_ECALL = 17;
constexpr unsigned int REG_S2 = 18;
constexpr unsigned int REG_S3 = 19;
constexpr unsigned int REG_S4 = 20;
constexpr unsigned int REG_S5 = 21;
constexpr unsigned int REG_S6 = 22;
constexpr unsigned int REG_S7 = 23;
constexpr unsigned int REG_S8 = 24;
constexpr unsigned int REG_S9 = 25;
constexpr unsigned int REG_S10 = 26;
constexpr unsigned int REG_S11 = 27;
constexpr unsigned int REG_T3 = 28;
constexpr unsigned int REG_T4 = 29;
constexpr unsigned int REG_T5 = 30;
constexpr unsigned int REG_T6 = 31;

constexpr unsigned int CSR_RO_BEGIN = 0xC00;
constexpr unsigned int CSR_RO_END = 0xC1F;

constexpr unsigned int CSR_H_RO_BEGIN = 0xC80;
constexpr unsigned int CSR_H_RO_END = 0xC9F;

/*
Cycle counter for RDCYCLE instruction.
*/
constexpr unsigned int CSR_CYCLE = 0xC00;

/*
Timer for RDTIME instruction.
*/
constexpr unsigned int CSR_TIME = 0xC01;

/*
Instructions-retired counter for RDINSTRET instruction.
*/
constexpr unsigned int CSR_INSTRET = 0xC02;

/*
Upper 32 bits of CSR_CYCLE, RV32I only.
*/
constexpr unsigned int CSR_CYCLEH = 0xC80;

/*
Upper 32 bits of CSR_TIME, RV32I only.
*/
constexpr unsigned int CSR_TIMEH = 0xC81;

/*
Upper 32 bits of CSR_INSTRET, RV32I only.
*/
constexpr unsigned int CSR_INSTRETH = 0xC82;
