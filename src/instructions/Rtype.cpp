#include "../Processor.hpp"

void Processor::ADD()
{
    WORD& dst = this->Register(this->rd);

    WORD result = this->Register(this->rs1) + this->Register(this->rs2);

    dst = result & ((1ULL << XLEN) - 1);
}

void Processor::SUB()
{
    WORD& dst = this->Register(this->rd);
    
    WORD result = this->Register(this->rs1) - this->Register(this->rs2);

    dst = result & ((1ULL << XLEN) - 1);
}

void Processor::SLL()
{
    WORD& dst = this->Register(this->rd);
    
    dst = this->Register(this->rs1) << this->Register(this->rs2);
}

void Processor::SLT()
{
    WORD& dst = this->Register(this->rd);

    dst = static_cast<int32_t>(this->Register(this->rs1)) < static_cast<int32_t>(this->Register(this->rs2));
}

void Processor::SLTU()
{
    WORD& dst = this->Register(this->rd);

    dst = this->Register(this->rs1) < this->Register(this->rs2);
}

void Processor::XOR()
{
    WORD& dst = this->Register(this->rd);

    dst = this->Register(this->rs1) ^ this->Register(this->rs2);
}

void Processor::SRL()
{
    WORD& dst = this->Register(this->rd);
    
    dst = this->Register(this->rs1) >> this->Register(this->rs2);
}

void Processor::SRA()
{
    WORD& dst = this->Register(this->rd);
    
    dst = static_cast<int32_t>(this->Register(this->rs1)) >> this->Register(this->rs2);
}

void Processor::OR()
{
    WORD& dst = this->Register(this->rd);

    dst = this->Register(this->rs1) | this->Register(this->rs2);
}

void Processor::AND()
{
    WORD& dst = this->Register(this->rd);

    dst = this->Register(this->rs1) & this->Register(this->rs2);
}
