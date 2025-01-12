#include "Emulator.hpp"
#include <print>
#include <format>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <elf.h>

Emulator::Emulator() : memory(RAM_SIZE), processor(&this->memory) {}

void Emulator::load(const std::vector<WORD> &program)
{
    if (program.size() * sizeof(WORD) >= RAM_SIZE)
        throw std::out_of_range(std::format("Memory capacity exceeded (program size: {}, max: {}).", program.size() * sizeof(WORD), RAM_SIZE));

    for (WORD i = 0; i < program.size(); i++)
        this->memory.write<WORD>(i * (IALIGN / 8), program[i]);
}

void Emulator::load_elf(const std::string &path)
{
    if (!std::filesystem::exists(path))
        throw std::invalid_argument("Invalid file path provided.");

    std::ifstream elf_file{path, std::ios::binary | std::ios::ate};

    long unsigned int elf_size = static_cast<long unsigned int>(elf_file.tellg());

    elf_file.seekg(0);

    if (elf_size < sizeof(Elf32_Ehdr))
        throw std::invalid_argument("Invalid file: Smaller than ELF.");

    Elf32_Ehdr elf_header;

    elf_file.read(reinterpret_cast<char *>(&elf_header), sizeof(Elf32_Ehdr));

    if (
        elf_header.e_ident[EI_MAG0] != ELFMAG0 || elf_header.e_ident[EI_MAG1] != ELFMAG1 || elf_header.e_ident[EI_MAG2] != ELFMAG2 || elf_header.e_ident[EI_MAG3] != ELFMAG3)
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
    elf_file.read(reinterpret_cast<char *>(&elf_section_header), sizeof(Elf32_Shdr));

    char *string_table = new char[elf_section_header.sh_size];

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
        elf_file.read(reinterpret_cast<char *>(&elf_section_header), sizeof(Elf32_Shdr));

        if (elf_section_header.sh_name)
        {
            std::string name = std::string{&string_table[elf_section_header.sh_name]};

            if (name == ".text")
            {
                WORD *text = new WORD[elf_section_header.sh_size];

                elf_file.seekg(elf_section_header.sh_offset);

                elf_file.read(reinterpret_cast<char *>(text), elf_section_header.sh_size);

                for (unsigned int j = 0; j < elf_section_header.sh_size; ++j)
                    this->memory.write(j * (IALIGN / 8), text[j]);

                delete[] text;
            }

            if (name == ".data")
            {
                BYTE *data = new BYTE[elf_section_header.sh_size];

                elf_file.seekg(elf_section_header.sh_offset);

                elf_file.read(reinterpret_cast<char *>(data), elf_section_header.sh_size);

                for (unsigned int j = 0; j < elf_section_header.sh_size; ++j)
                    this->memory.write(elf_section_header.sh_addr + j + (IALIGN / 8), data[j]);

                delete[] data;
            }
        }
    }

    delete[] string_table;

    elf_file.close();
}

void Emulator::run()
{
    while (!this->processor.isStopped())
    {
        try
        {
            this->processor.step();
        }
        catch (const std::overflow_error &e)
        {
            std::println("Emulator stopped with `overflow_error`: {}", e.what());
            break;
        }
        catch (const std::runtime_error &e)
        {
            std::println("Emulator stopped with `runtime_error`: {}", e.what());
            break;
        }
    }
}

void Emulator::reset()
{
    this->processor.reset();
    this->memory.reset();
}

void Emulator::state() const
{
    std::println("\t\t==== EMULATOR STATE ====");
    std::println("");

    this->processor.state();
}
