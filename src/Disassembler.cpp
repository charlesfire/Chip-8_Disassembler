#include "Disassembler.hpp"
#include <functional>

Disassembler::Disassembler()
{
    mapper[0] = &Disassembler::Sys0xNNN;
}

Disassembler::~Disassembler()
{

}

bool Disassembler::LoadAssembly(const std::string& path)
{

}

void Disassembler::Disassemble(std::ostream& out)
{

}

void Disassembler::Sys0xNNN(std::ostream& out, uint8_t b1, uint8_t b2, uint8_t b3)
{
    out << "SYS 0x" << std::hex << b3 << b2 << b1;
}
