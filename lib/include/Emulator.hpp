#pragma once
#include <string>
#include <vector>
#include "types.hpp"
#include "Processor.hpp"
#include "Memory.hpp"

class Emulator
{
public:
    Emulator();

    void load(const std::vector<WORD> &program);
    void load_elf(const std::string &path);
    void run();

    void reset();
    void state() const;

private:
    Processor processor;
    Memory memory;
};
