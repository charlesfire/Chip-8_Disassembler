#include "Assembler.hpp"
#include <iostream>

Assembler::Assembler() : lines(), mapper(), validInstructions()
{
    validInstructions[0] = std::regex("SYS 0x([0-9a-f]{1,3})", std::regex::icase); mapper[0] = &Assembler::Sys0xNNN;         /* 0NNN */
    validInstructions[1] = std::regex("CLEAR", std::regex::icase); mapper[1] = &Assembler::Clear;         /* 00E0 */
    validInstructions[2] = std::regex("RET", std::regex::icase); mapper[2] = &Assembler::Ret;         /* 00EE */
    validInstructions[3] = std::regex("JMP 0x([0-9a-f]{1,3})", std::regex::icase); mapper[3] = &Assembler::Jmp0xNNN;         /* 1NNN */
    validInstructions[4] = std::regex("CALL 0x([0-9a-f]{1,3})", std::regex::icase); mapper[4] = &Assembler::Call0xNNN;         /* 2NNN */
    validInstructions[5] = std::regex("SKE V([0-9a-f]) 0x([0-9a-f]{1,2})", std::regex::icase); mapper[5] = &Assembler::SkeVX0xNN;         /* 3XNN */
    validInstructions[6] = std::regex("SKNE V([0-9a-f]) 0x([0-9a-f]{1,2})", std::regex::icase); mapper[6] = &Assembler::SkneVX0xNN;         /* 4XNN */
    validInstructions[7] = std::regex("SKE V([0-9a-f]) V([0-9a-f])", std::regex::icase); mapper[7] = &Assembler::SkeVXVY;         /* 5XY0 */
    validInstructions[8] = std::regex("LD V([0-9a-f]) 0x([0-9a-f]{1,2})", std::regex::icase); mapper[8] = &Assembler::LdVX0xNN;         /* 6XNN */
    validInstructions[9] = std::regex("ADD V([0-9a-f]) 0x([0-9a-f]{1,2})", std::regex::icase); mapper[9] = &Assembler::AddVX0xNN;         /* 7XNN */
    validInstructions[10] = std::regex("LD V([0-9a-f]) V([0-9a-f])", std::regex::icase); mapper[10] = &Assembler::LdVXVY;       /* 8XY0 */
    validInstructions[11] = std::regex("OR V([0-9a-f]) V([0-9a-f])", std::regex::icase); mapper[11] = &Assembler::OrVXVY;       /* 8XY1 */
    validInstructions[12] = std::regex("AND V([0-9a-f]) V([0-9a-f])", std::regex::icase); mapper[12] = &Assembler::AndVXVY;       /* 8XY2 */
    validInstructions[13] = std::regex("XOR V([0-9a-f]) V([0-9a-f])", std::regex::icase); mapper[13] = &Assembler::XorVXVY;       /* BXY3 */
    validInstructions[14] = std::regex("ADD V([0-9a-f]) V([0-9a-f])", std::regex::icase); mapper[14] = &Assembler::AddVXVY;       /* 8XY4 */
    validInstructions[15] = std::regex("SUB V([0-9a-f]) V([0-9a-f])", std::regex::icase); mapper[15] = &Assembler::SubVXVY;       /* 8XY5 */
    validInstructions[16] = std::regex("SHR V([0-9a-f])", std::regex::icase); mapper[16] = &Assembler::ShrVX;       /* 8XY6 */
    validInstructions[17] = std::regex("SUBN V([0-9a-f]) V([0-9a-f])", std::regex::icase); mapper[17] = &Assembler::SubnVXVY;       /* 8XY7 */
    validInstructions[18] = std::regex("SHL V([0-9a-f])", std::regex::icase); mapper[18] = &Assembler::ShlVX;       /* 8XYE */
    validInstructions[19] = std::regex("SKNE V([0-9a-f]) V([0-9a-f])", std::regex::icase); mapper[19] = &Assembler::SkneVXVY;       /* 9XY0 */
    validInstructions[20] = std::regex("LD I 0x([0-9a-f]{1,3})", std::regex::icase); mapper[20] = &Assembler::LdI0xNNN;       /* ANNN */
    validInstructions[21] = std::regex("JMP V0 0x([0-9a-f]{1,3})", std::regex::icase); mapper[21] = &Assembler::JmpV00xNNN;       /* BNNN */
    validInstructions[22] = std::regex("RND V([0-9a-f]) 0x([0-9a-f]{1,2})", std::regex::icase); mapper[22] = &Assembler::RndVX0xNN;       /* CXNN */
    validInstructions[23] = std::regex("DRW V([0-9a-f]) V([0-9a-f]) 0x([0-9a-f])", std::regex::icase); mapper[23] = &Assembler::DrwVXVY0xN;       /* DXYN */
    validInstructions[24] = std::regex("SKP V([0-9a-f])", std::regex::icase); mapper[24] = &Assembler::SkpVX;       /* EX9E */
    validInstructions[25] = std::regex("SKNP V([0-9a-f])", std::regex::icase); mapper[25] = &Assembler::SknpVX;       /* EXA1 */
    validInstructions[26] = std::regex("LD V([0-9a-f]) DT", std::regex::icase); mapper[26] = &Assembler::LdVXDT;       /* FX07 */
    validInstructions[27] = std::regex("LD V([0-9a-f]) K", std::regex::icase); mapper[27] = &Assembler::LdVXK;       /* FX0A */
    validInstructions[28] = std::regex("LD DT V([0-9a-f])", std::regex::icase); mapper[28] = &Assembler::LdDTVX;       /* FX15 */
    validInstructions[29] = std::regex("LD ST V([0-9a-f])", std::regex::icase); mapper[29] = &Assembler::LdSTVX;       /* FX18 */
    validInstructions[30] = std::regex("ADD I V([0-9a-f])", std::regex::icase); mapper[30] = &Assembler::AddIVX;       /* FX1E */
    validInstructions[31] = std::regex("LD N V([0-9a-f])", std::regex::icase); mapper[31] = &Assembler::LdNVX;       /* FX29 */
    validInstructions[32] = std::regex("LD D V([0-9a-f])", std::regex::icase); mapper[32] = &Assembler::LdDVX;       /* FX33 */
    validInstructions[33] = std::regex("LD M V([0-9a-f])", std::regex::icase); mapper[33] = &Assembler::LdMVX;       /* FX55 */
    validInstructions[34] = std::regex("LD V([0-9a-f]) M", std::regex::icase); mapper[34] = &Assembler::LdVXM;       /* FX65 */
}

bool Assembler::LoadCode(std::istream& in)
{
    std::string line;
    while (std::getline(in, line))
    {
        lines.push_back(line);
    }

    return true;
}

bool Assembler::Assemble(std::ostream& out)
{
    for (auto line : lines)
    {
        bool hasMatched = false;
        for (unsigned int i(0); i < validInstructions.size(); i++)
        {
            std::smatch match;
            if (std::regex_match(line, match, validInstructions[i]))
            {
                mapper[i](this, out, match);
                hasMatched = true;
            }
        }

        if (!hasMatched)
        {
            std::cerr << "Error : unrecognized instruction : " << line << std::endl;
            return false;
        }
    }
    return true;
}

void Assembler::Sys0xNNN(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = std::stoi(results[1], 0, 16);
    unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::Clear(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x00E0;
    unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::Ret(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x00EE;
    unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::Jmp0xNNN(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x1000 + std::stoi(results[1], 0, 16);
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::Call0xNNN(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x2000 + std::stoi(results[1], 0, 16);
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::SkeVX0xNN(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x3000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16);
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::SkneVX0xNN(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x4000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16);
	unsigned char high = opcode >> 8;
	unsigned char low = opcode << 4;
	low = low >> 4;	out << high << low;
}

void Assembler::SkeVXVY(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x5000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16) * 0x10;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::LdVX0xNN(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x6000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16);
    unsigned char high = opcode >> 8;
    unsigned char low = (opcode << 8) >> 8;

    out << high << low;
}

void Assembler::AddVX0xNN(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x7000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16);
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::LdVXVY(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x8000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16) * 0x10;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::OrVXVY(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x8000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16) * 0x10 + 0x1;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::AndVXVY(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x8000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16) * 0x10 + 0x2;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::XorVXVY(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x8000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16) * 0x10 + 0x3;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::AddVXVY(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x8000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16) * 0x10 + 0x4;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::SubVXVY(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x8000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16) * 0x10 + 0x5;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::ShrVX(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x8000 + std::stoi(results[1], 0, 16) * 0x100 + 0x6;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::SubnVXVY(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x8000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16) * 0x10 + 0x7;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::ShlVX(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x8000 + std::stoi(results[1], 0, 16) * 0x100 + 0xE;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::SkneVXVY(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0x9000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16) * 0x10;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::LdI0xNNN(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xA000 + std::stoi(results[1], 0, 16);
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::JmpV00xNNN(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xB000 + std::stoi(results[1], 0, 16);
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::RndVX0xNN(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xC000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16);
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::DrwVXVY0xN(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xD000 + std::stoi(results[1], 0, 16) * 0x100 + std::stoi(results[2], 0, 16) * 0x10 + std::stoi(results[3], 0, 16);
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::SkpVX(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xE000 + std::stoi(results[1], 0, 16) * 0x100 + 0x9E;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::SknpVX(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xE000 + std::stoi(results[1], 0, 16) * 0x100 + 0xA1;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::LdVXDT(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xF000 + std::stoi(results[1], 0, 16) * 0x100 + 0x07;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::LdVXK(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xF000 + std::stoi(results[1], 0, 16) * 0x100 + 0x0A;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::LdDTVX(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xF000 + std::stoi(results[1], 0, 16) * 0x100 + 0x15;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::LdSTVX(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xF000 + std::stoi(results[1], 0, 16) * 0x100 + 0x18;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::AddIVX(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xF000 + std::stoi(results[1], 0, 16) * 0x100 + 0x1E;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::LdNVX(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xF000 + std::stoi(results[1], 0, 16) * 0x100 + 0x29;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::LdDVX(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xF000 + std::stoi(results[1], 0, 16) * 0x100 + 0x33;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::LdMVX(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xF000 + std::stoi(results[1], 0, 16) * 0x100 + 0x55;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}

void Assembler::LdVXM(std::ostream& out, const std::smatch& results)
{
    uint16_t opcode = 0xF000 + std::stoi(results[1], 0, 16) * 0x100 + 0x65;
	unsigned char high = opcode >> 8;
	unsigned char low = (opcode << 8) >> 8;
	out << high << low;
}
