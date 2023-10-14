#include "../Processor.hpp"
#include "../utils.hpp"

void Processor::LUI()
{
    WORD& dst = this->Register(this->rd);

    dst = signextend(this->imm, 20);
}

void Processor::AUIPC()
{
    WORD& dst = this->Register(this->rd);

    dst = this->pc + signextend(this->imm, 20);
}
