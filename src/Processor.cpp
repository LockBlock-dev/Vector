#include "Processor.hpp"
#include "constants.hpp"
#include <fstream>
#include <iostream>
#include <bitset>
#include <elf.h>

void Processor::load(const std::vector<WORD>& program)
{
    this->memory.load(program);
}

void Processor::load_elf(const std::string& path)
{
    std::ifstream elf_file { path, std::ios::binary | std::ios::ate };

    long unsigned int elf_size = static_cast<long unsigned int>(elf_file.tellg());

    elf_file.seekg(0);

    if (elf_size < sizeof(Elf32_Ehdr)) return;

    Elf32_Ehdr elf_header;

    elf_file.read(reinterpret_cast<char*>(&elf_header), sizeof(Elf32_Ehdr));

    if (
        elf_header.e_ident[EI_MAG0] != ELFMAG0
        || elf_header.e_ident[EI_MAG1] != ELFMAG1
        || elf_header.e_ident[EI_MAG2] != ELFMAG2
        || elf_header.e_ident[EI_MAG3] != ELFMAG3
    )
        throw std::invalid_argument("Invalid file: Wrong magic.");

    if (elf_header.e_ident[EI_CLASS] != ELFCLASS32)
        throw std::invalid_argument("Invalid ELF file: Only 32-bits objects are supported.");

    if (elf_header.e_type != ET_EXEC && elf_header.e_type != ET_DYN)
        throw std::invalid_argument("Invalid ELF file: Not an executable.");

    if (elf_header.e_machine != EM_RISCV)
        throw std::invalid_argument("Invalid ELF file: Wrong architecture.");

    if (elf_header.e_flags & EF_RISCV_RVC)
        throw std::invalid_argument("Invalid ELF file: C extension is not supported.");

    if (elf_header.e_flags & EF_RISCV_RVE)
        throw std::invalid_argument("Invalid ELF file: Embedded Instruction Set is not supported.");

    if (elf_header.e_phnum <= 0)
        throw std::invalid_argument("Invalid ELF file: No program headers.");

    if (elf_header.e_shnum <= 0)
        throw std::invalid_argument("Invalid ELF file: No section headers.");

    Elf32_Shdr elf_section_header;

    // Go to the string table section header
    elf_file.seekg(elf_header.e_shoff + elf_header.e_shstrndx * sizeof(Elf32_Shdr));

    // Read the string table section header
    elf_file.read(reinterpret_cast<char*>(&elf_section_header), sizeof(Elf32_Shdr));

    char* string_table = new char[elf_section_header.sh_size];

    // Go to the string table section
    elf_file.seekg(elf_section_header.sh_offset);

    // Read the string table section
    elf_file.read(string_table, elf_section_header.sh_size);

    // For each sections
    for (unsigned int i = 0; i < elf_header.e_shnum; ++i)
    {
        // Go to the section header
        elf_file.seekg(elf_header.e_shoff + i * sizeof(Elf32_Shdr));

        // Read section header
        elf_file.read(reinterpret_cast<char*>(&elf_section_header), sizeof(Elf32_Shdr));

        if (elf_section_header.sh_name)
        {
            std::string name = std::string {&string_table[elf_section_header.sh_name]};

            if (name == ".text")
            {
                WORD* text = new WORD[elf_section_header.sh_size];

                elf_file.seekg(elf_section_header.sh_offset);

                elf_file.read(reinterpret_cast<char*>(text), elf_section_header.sh_size);

                for (unsigned int j = 0; j < elf_section_header.sh_size; ++j)
                    this->memory.write(j * (IALIGN / 8), text[j]);

                delete[] text;
            }

            if (name == ".data")
            {
                BYTE* data = new BYTE[elf_section_header.sh_size];

                elf_file.seekg(elf_section_header.sh_offset);

                elf_file.read(reinterpret_cast<char*>(data), elf_section_header.sh_size);

                // std::cout << data << "\n";

                for (unsigned int j = 0; j < elf_section_header.sh_size; ++j)
                    this->memory.write(elf_section_header.sh_addr + j + (IALIGN / 8), data[j]);

                delete[] data;
            }
        }
    }

    delete[] string_table;

    elf_file.close();
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
