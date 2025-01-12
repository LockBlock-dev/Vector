#pragma once
#include "instructions/Instruction32.hpp"

class Processor;
class Memory;

class ExecutionUnit
{
public:
    ExecutionUnit(Processor *proc, Memory *memory);

    void execute(const Instruction32 &instruction);

private:
    Processor *proc;
    Memory *mem;
    Instruction32 instr;

    /* R-type */

    void OP();

    /* I-type */

    void OP_IMM();

    void LOAD();

    void JALR();

    void MISC_MEM();

    void SYSTEM();

    /* S-type */

    void STORE();

    /* B-type */

    void BRANCH();

    /* U-type */

    void LUI();

    void AUIPC();

    /* J-type */

    void JAL();
};
