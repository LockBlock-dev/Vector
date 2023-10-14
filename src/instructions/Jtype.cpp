#include "../Processor.hpp"
#include "../utils.hpp"
#include <stdexcept>
#include <format>

void Processor::JAL()
{
    WORD& dst = this->Register(this->rd);

    WORD offset = signextend(this->imm, 20);

    if (offset % 4 != 0)
        throw std::runtime_error("Instruction address misaligned (offset: 0x" + std::format("{:X}", offset) + ").");

    dst = this->pc + 4;

    // PC is incremented post instruction execution
    this->pc += offset - 4;
}
