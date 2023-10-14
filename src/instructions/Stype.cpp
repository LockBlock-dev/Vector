#include "../Processor.hpp"
#include "../utils.hpp"

void Processor::SB()
{
    WORD address = this->Register(this->rs1) + signextend(this->imm);

    this->memory.write<BYTE>(
        address,
        static_cast<BYTE>(this->Register(this->rs2))
    );
}

void Processor::SH()
{
    WORD address = this->Register(this->rs1) + signextend(this->imm);

    this->memory.write<HALFWORD>(
        address,
        static_cast<HALFWORD>(this->Register(this->rs2))
    );
}

void Processor::SW()
{
    WORD address = this->Register(this->rs1) + signextend(this->imm);

    this->memory.write<WORD>(
        address,
        this->Register(this->rs2)
    );
}
