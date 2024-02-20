#pragma once
#include <string>
#include "Memory.hpp"
#include "Registers.hpp"


class Processor {
public:
    void load(const std::vector<WORD>& program);
    void load_elf(const std::string& path);
    void step();
    void reset();
    void state() const;
    
    WORD& Register(const WORD index);
    const WORD& Register(const WORD index) const;
private:
    WORD pc = 0;

    WORD rd = 0;
    WORD rs1 = 0;
    WORD rs2 = 0;
    WORD imm = 0;

    Registers registers {};
    Memory memory {};

    void execute(WORD instruction);

    /* R-type */

    void ADD();
    void SUB();
    void SLL();
    void SLT();
    void SLTU();
    void XOR();
    void SRL();
    void SRA();
    void OR();
    void AND();

    /* I-type */

    void ADDI();
    void SLTI();
    void SLTIU();
    void XORI();
    void ORI();
    void ANDI();
    void SLLI();
    void SRLI();
    void SRAI();

    void LB();
    void LH();
    void LW();
    void LBU();
    void LHU();

    void JALR();

    void ECALL();
    void EBREAK();

    /* S-type */

    void SB();
    void SH();
    void SW();

    /* B-type */

    void BEQ();
    void BNE();
    void BLT();
    void BGE();
    void BLTU();
    void BGEU();

    /* U-type */

    void LUI();
    void AUIPC();

    /* J-type */

    void JAL();
};
