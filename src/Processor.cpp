#include "Processor.hpp"
#include "constants.hpp"
#include <iostream>
#include <bitset>

void Processor::load(const std::vector<WORD>& program)
{
    this->memory.load(program);
}

void Processor::step()
{
    WORD instruction = this->memory.read<WORD>(this->pc);

    this->execute(instruction);

    // Add the instruction size to the program counter
    // 4 bytes (RVI) or 2 bytes (RVC)
    this->pc += (IALIGN / 8);
}

void Processor::reset()
{
    this->pc = 0;
    this->registers = {};
}

WORD& Processor::Register(const WORD index)
{
    return this->registers.get(static_cast<BYTE>(index));
}

const WORD& Processor::Register(const WORD index) const
{
    return this->registers.get(static_cast<BYTE>(index));
}

void Processor::state() const
{
    std::cout << "\t\t====== CPU STATE" << " ======\n";

    std::cout << "pc = " << this->pc << "\n";

    std::cout << "\n";

    this->registers.state();
}

void Processor::execute(WORD instruction)
{
    BYTE opcode = (instruction & 0b1111111);

    switch (opcode)
    {
        case RV32I_LOAD: {
            std::cout << "RV32I_LOAD\n";

            struct IType* instr = reinterpret_cast<IType*>(&instruction);

            this->rd = instr->rd;
            this->rs1 = instr->rs1;
            this->imm = instr->imm;

            switch(instr->funct3)
            {
                // LB
                case 0b000: {
                    this->LB();
                    break;
                }
                // LH
                case 0b001: {
                    this->LB();
                    break;
                }
                // LW
                case 0b010: {
                    this->LW();
                    break;
                }
                // LBU
                case 0b100: {
                    this->LBU();
                    break;
                }
                // LHU
                case 0b101: {
                    this->LHU();
                    break;
                }

                default: {
                    std::bitset<3> funct3Binary{instr->funct3};
                    throw std::runtime_error("Unhandled RV32I_LOAD funct3 (funct3: 0b" + funct3Binary.to_string() + ").");
                }
            }

            break;
        }
        
        case RV32I_OP_IMM: {
            std::cout << "RV32I_OP_IMM\n";

            struct IType* instr = reinterpret_cast<IType*>(&instruction);

            this->rd = instr->rd;
            this->rs1 = instr->rs1;
            this->imm = instr->imm;

            switch(instr->funct3)
            {
                // ADDI
                case 0b000: {
                    this->ADDI();
                    break;
                }
                // SLLI
                case 0b001: {
                    this->SLLI();
                    break;
                }
                // SLTI
                case 0b010: {
                    this->SLTI();
                    break;
                }
                // SLTIU
                case 0b011: {
                    this->SLTIU();
                    break;
                }
                // XORI
                case 0b100: {
                    this->XORI();
                    break;
                }
                // SRLI / SRAI
                case 0b101: {
                    // the right shift type is encoded in bit 30
                    switch (this->imm & 0x40000000)
                    {
                        // SRLI
                        case 0: {
                            this->SRLI();
                            break;
                        }
                        // SRAI
                        case 1: {
                            this->SRAI();
                            break;
                        }
                    }

                    break;
                }
                // ORI
                case 0b110: {
                    this->ORI();
                    break;
                }
                // ANDI
                case 0b111: {
                    this->ANDI();
                    break;
                }

                default: {
                    std::bitset<3> funct3Binary{instr->funct3};
                    throw std::runtime_error("Unhandled RV32I_OP_IMM funct3 (funct3: 0b" + funct3Binary.to_string() + ").");
                }
            }

            break;
        }

        case RV32I_AUIPC: {
            std::cout << "RV32I_AUIPC\n";

            struct UType* instr = reinterpret_cast<UType*>(&instruction);

            this->rd = instr->rd;
            this->imm = instr->imm << 12;

            this->AUIPC();

            break;
        }

        case RV32I_STORE: {
            std::cout << "RV32I_STORE\n";

            struct SType* instr = reinterpret_cast<SType*>(&instruction);

            this->imm = instr->imm1 | (instr->imm2 << 5);
            this->rs1 = instr->rs1;
            this->rs2 = instr->rs2;

            switch(instr->funct3)
            {
                // SB
                case 0b000: {
                    this->SB();
                    break;
                }
                // SH
                case 0b001: {
                    this->SH();
                    break;
                }
                // SW
                case 0b010: {
                    this->SW();
                    break;
                }

                default: {
                    std::bitset<3> funct3Binary{instr->funct3};
                    throw std::runtime_error("Unhandled RV32I_STORE funct3 (funct3: 0b" + funct3Binary.to_string() + ").");
                }
            }

            break;
        }

        case RV32I_OP: {
            std::cout << "RV32I_OP\n";

            struct RType* instr = reinterpret_cast<RType*>(&instruction);

            this->rd = instr->rd;
            this->rs1 = instr->rs1;
            this->rs2 = instr->rs2;

            switch(instr->funct3)
            {
                // ADD / SUB
                case 0b000: {
                    switch (instr->funct7)
                    {
                        // ADD
                        case 0b0000000: {
                            this->ADD();
                            break;
                        }
                        // SUB
                        case 0b0100000: {
                            this->SUB();
                            break;
                        }
                    }

                    break;                    
                } 
                // SLL
                case 0b001: {
                    this->SLL();
                    break;
                }
                // SLT
                case 0b010: {
                    this->SLT();
                    break;
                }
                // SLTU
                case 0b011: {
                    this->SLTU();
                    break;
                }
                // XOR
                case 0b100: {
                    this->XOR();
                    break;
                }
                // SRL / SRA
                case 0b101: {
                    switch (instr->funct7)
                    {
                        // SRL
                        case 0b0000000: {
                            this->SRL();
                            break;
                        }
                        // SRA
                        case 0b0100000: {
                            this->SRA();
                            break;
                        }
                    }

                    break;   
                }
                // OR
                case 0b110: {
                    this->OR();
                    break;
                }
                // AND
                case 0b111: {
                    this->AND();
                    break;
                }

                default: {
                    std::bitset<3> funct3Binary{instr->funct3};
                    throw std::runtime_error("Unhandled RV32I_OP funct3 (funct3: 0b" + funct3Binary.to_string() + ").");
                }
            }

            break;
        }
        
        case RV32I_LUI: {
            std::cout << "RV32I_LUI\n";

            struct UType* instr = reinterpret_cast<UType*>(&instruction);

            this->rd = instr->rd;
            this->imm = instr->imm << 12;

            this->LUI();

            break;
        }

        case RV32I_BRANCH: {
            std::cout << "RV32I_BRANCH\n";

            struct BType* instr = reinterpret_cast<BType*>(&instruction);

            this->imm = (instr->imm2 << 1) | (instr->imm3 << 5) | (instr->imm1 << 11) | (instr->imm4 << 12);
            this->rs1 = instr->rs1;
            this->rs2 = instr->rs2;

            switch(instr->funct3)
            {
                // BEQ
                case 0b000: {
                    this->BEQ();
                    break;
                }
                // BNE
                case 0b001: {
                    this->BNE();
                    break;
                }
                // BLT
                case 0b100: {
                    this->BLT();
                    break;
                }
                // BGE
                case 0b101: {
                    this->BGE();
                    break;
                }
                // BLTU
                case 0b110: {
                    this->BLTU();
                    break;
                }
                // BGEU
                case 0b111: {
                    this->BGEU();
                    break;
                }

                default: {
                    std::bitset<3> funct3Binary{instr->funct3};
                    throw std::runtime_error("Unhandled RV32I_BRANCH funct3 (funct3: 0b" + funct3Binary.to_string() + ").");
                }
            }

            break;
        }

        case RV32I_JALR: {
            std::cout << "RV32I_JALR\n";

            struct IType* instr = reinterpret_cast<IType*>(&instruction);

            this->rd = instr->rd;
            this->rs1 = instr->rs1;
            this->imm = instr->imm;

            this->JALR();

            break;
        }

        case RV32I_JAL: {
            std::cout << "RV32I_JAL\n";

            struct JType* instr = reinterpret_cast<JType*>(&instruction);

            this->rd = instr->rd;
            this->imm = (instr->imm3 << 1) | (instr->imm2 << 11) | (instr->imm1 << 12) | (instr->imm4 << 20);

            this->JAL();

            break;
        }

        case RV32I_SYSTEM: {
            std::cout << "RV32I_SYSTEM\n";

            struct IType* instr = reinterpret_cast<IType*>(&instruction);

            this->rd = instr->rd;
            this->rs1 = instr->rs1;
            // funct12
            this->imm = instr->imm;

            switch(instr->funct3)
            {
                case 0: {
                    switch (this->imm)
                    {
                        // ECALL
                        case 0: {
                            this->ECALL();
                            break;
                        }
                        // EBREAK
                        case 1: {
                            this->EBREAK();
                            break;
                        }
                    }

                    break;
                }

                default: {
                    std::bitset<3> funct3Binary{instr->funct3};
                    throw std::runtime_error("Unhandled RV32I_SYSTEM funct3 (funct3: 0b" + funct3Binary.to_string() + ").");
                }
            }

            break;
        }

        default: {
            std::bitset<7> opcodeBinary{opcode};
            throw std::runtime_error("Unhandled operation (op code: 0b" + opcodeBinary.to_string() + ").");
        }
    }
}
