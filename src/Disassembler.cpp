#include "Disassembler.hpp"
#include <iostream>
#include <fstream>
#include <functional>
#include "OpcodesMatcher.hpp"

std::array<uint16_t, Disassembler::Opcodes::NBOPCODE> Disassembler::Opcodes::mask;
std::array<int, Disassembler::Opcodes::NBOPCODE> Disassembler::Opcodes::id;

Disassembler::Disassembler() : program(), mapper()
{
    Disassembler::Opcodes::mask[0] = 0x0000; Disassembler::Opcodes::id[0] = 0x0FFF; mapper[0] = &Disassembler::Sys0xNNN;         /* 0NNN */
    Disassembler::Opcodes::mask[1] = 0xFFFF; Disassembler::Opcodes::id[1] = 0x00E0; mapper[1] = &Disassembler::Clear;         /* 00E0 */
    Disassembler::Opcodes::mask[2] = 0xFFFF; Disassembler::Opcodes::id[2] = 0x00EE; mapper[2] = &Disassembler::Ret;         /* 00EE */
    Disassembler::Opcodes::mask[3] = 0xF000; Disassembler::Opcodes::id[3] = 0x1000; mapper[3] = &Disassembler::Jmp0xNNN;         /* 1NNN */
    Disassembler::Opcodes::mask[4] = 0xF000; Disassembler::Opcodes::id[4] = 0x2000; mapper[4] = &Disassembler::Call0xNNN;         /* 2NNN */
    Disassembler::Opcodes::mask[5] = 0xF000; Disassembler::Opcodes::id[5] = 0x3000; mapper[5] = &Disassembler::SkeVX0xNN;         /* 3XNN */
    Disassembler::Opcodes::mask[6] = 0xF000; Disassembler::Opcodes::id[6] = 0x4000; mapper[6] = &Disassembler::SkneVX0xNN;         /* 4XNN */
    Disassembler::Opcodes::mask[7] = 0xF00F; Disassembler::Opcodes::id[7] = 0x5000; mapper[7] = &Disassembler::SkeVXVY;         /* 5XY0 */
    Disassembler::Opcodes::mask[8] = 0xF000; Disassembler::Opcodes::id[8] = 0x6000; mapper[8] = &Disassembler::LdVX0xNN;         /* 6XNN */
    Disassembler::Opcodes::mask[9] = 0xF000; Disassembler::Opcodes::id[9] = 0x7000; mapper[9] = &Disassembler::AddVX0xNN;         /* 7XNN */
    Disassembler::Opcodes::mask[10] = 0xF00F; Disassembler::Opcodes::id[10] = 0x8000; mapper[10] = &Disassembler::LdVXVY;       /* 8XY0 */
    Disassembler::Opcodes::mask[11] = 0xF00F; Disassembler::Opcodes::id[11] = 0x8001; mapper[11] = &Disassembler::OrVXVY;       /* 8XY1 */
    Disassembler::Opcodes::mask[12] = 0xF00F; Disassembler::Opcodes::id[12] = 0x8002; mapper[12] = &Disassembler::AndVXVY;       /* 8XY2 */
    Disassembler::Opcodes::mask[13] = 0xF00F; Disassembler::Opcodes::id[13] = 0x8003; mapper[13] = &Disassembler::XorVXVY;       /* BXY3 */
    Disassembler::Opcodes::mask[14] = 0xF00F; Disassembler::Opcodes::id[14] = 0x8004; mapper[14] = &Disassembler::AddVXVY;       /* 8XY4 */
    Disassembler::Opcodes::mask[15] = 0xF00F; Disassembler::Opcodes::id[15] = 0x8005; mapper[15] = &Disassembler::SubVXVY;       /* 8XY5 */
    Disassembler::Opcodes::mask[16] = 0xF00F; Disassembler::Opcodes::id[16] = 0x8006; mapper[16] = &Disassembler::ShrVX;       /* 8XY6 */
    Disassembler::Opcodes::mask[17] = 0xF00F; Disassembler::Opcodes::id[17] = 0x8007; mapper[17] = &Disassembler::SubnVXVY;       /* 8XY7 */
    Disassembler::Opcodes::mask[18] = 0xF00F; Disassembler::Opcodes::id[18] = 0x800E; mapper[18] = &Disassembler::ShlVX;       /* 8XYE */
    Disassembler::Opcodes::mask[19] = 0xF00F; Disassembler::Opcodes::id[19] = 0x9000; mapper[19] = &Disassembler::SkneVXVY;       /* 9XY0 */
    Disassembler::Opcodes::mask[20] = 0xF000; Disassembler::Opcodes::id[20] = 0xA000; mapper[20] = &Disassembler::LdI0xNNN;       /* ANNN */
    Disassembler::Opcodes::mask[21] = 0xF000; Disassembler::Opcodes::id[21] = 0xB000; mapper[21] = &Disassembler::JmpV00xNNN;       /* BNNN */
    Disassembler::Opcodes::mask[22] = 0xF000; Disassembler::Opcodes::id[22] = 0xC000; mapper[22] = &Disassembler::RndVX0xNN;       /* CXNN */
    Disassembler::Opcodes::mask[23] = 0xF000; Disassembler::Opcodes::id[23] = 0xD000; mapper[23] = &Disassembler::DrwVXVY0xN;       /* DXYN */
    Disassembler::Opcodes::mask[24] = 0xF0FF; Disassembler::Opcodes::id[24] = 0xE09E; mapper[24] = &Disassembler::SkpVX;       /* EX9E */
    Disassembler::Opcodes::mask[25] = 0xF0FF; Disassembler::Opcodes::id[25] = 0xE0A1; mapper[25] = &Disassembler::SknpVX;       /* EXA1 */
    Disassembler::Opcodes::mask[26] = 0xF0FF; Disassembler::Opcodes::id[26] = 0xF007; mapper[26] = &Disassembler::LdVXDT;       /* FX07 */
    Disassembler::Opcodes::mask[27] = 0xF0FF; Disassembler::Opcodes::id[27] = 0xF00A; mapper[27] = &Disassembler::LdVXK;       /* FX0A */
    Disassembler::Opcodes::mask[28] = 0xF0FF; Disassembler::Opcodes::id[28] = 0xF015; mapper[28] = &Disassembler::LdDTVX;       /* FX15 */
    Disassembler::Opcodes::mask[29] = 0xF0FF; Disassembler::Opcodes::id[29] = 0xF018; mapper[29] = &Disassembler::LdSTVX;       /* FX18 */
    Disassembler::Opcodes::mask[30] = 0xF0FF; Disassembler::Opcodes::id[30] = 0xF01E; mapper[30] = &Disassembler::AddIVX;       /* FX1E */
    Disassembler::Opcodes::mask[31] = 0xF0FF; Disassembler::Opcodes::id[31] = 0xF029; mapper[31] = &Disassembler::LdNVX;       /* FX29 */
    Disassembler::Opcodes::mask[32] = 0xF0FF; Disassembler::Opcodes::id[32] = 0xF033; mapper[32] = &Disassembler::LdDVX;       /* FX33 */
    Disassembler::Opcodes::mask[33] = 0xF0FF; Disassembler::Opcodes::id[33] = 0xF055; mapper[33] = &Disassembler::LdMVX;       /* FX55 */
    Disassembler::Opcodes::mask[34] = 0xF0FF; Disassembler::Opcodes::id[34] = 0xF065; mapper[34] = &Disassembler::LdVXM;       /* FX65 */
}

bool Disassembler::LoadAssembly(const std::string& path)
{
    std::ifstream file;
    std::streampos fileSize;
    char* buffer = nullptr;
    file.open(path, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        fileSize = file.tellg();
        if (fileSize % 2)
            return false;
        buffer = new char [fileSize];
        file.seekg (0, std::ios::beg);
        file.read (buffer, fileSize);
        file.close();

        for (int i(0); i < fileSize; ++i)
        {
            program.push_back((buffer[i] << 8) + buffer[++i]);
        }

        delete[] buffer;
        return true;
    }
    return false;
}

bool Disassembler::Disassemble(std::ostream& out)
{
    for (auto opcode : program)
    {
        int opcodeId = OpcodesMatcher<Disassembler::Opcodes>::getActionId(opcode);
        if (opcodeId < 0 || opcodeId > 34)
        {
            std::cerr << "Unknown opcode : 0x" << std::hex << opcode;
            return false;
        }

        uint8_t b1, b2, b3;
        b3 = (opcode&(0x0F00)) >> 8;
        b2 = (opcode&(0x00F0)) >> 4;
        b1 = (opcode&(0x000F));

        mapper[opcodeId](this, out, b1, b2, b3);
    }

    return true;
}

void Disassembler::Sys0xNNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "SYS 0x" << std::hex << (int)b3 << (int)b2 << (int)b1 << std::endl;
}

void Disassembler::Clear(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "CLEAR" << std::endl;
}

void Disassembler::Ret(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "RET" << std::endl;
}

void Disassembler::Jmp0xNNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "JMP 0x" << std::hex << (int)b3 << (int)b2 << (int)b1 << std::endl;
}

void Disassembler::Call0xNNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "CALL 0x" << std::hex << (int)b3 << (int)b2 << (int)b1 << std::endl;
}

void Disassembler::SkeVX0xNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "SKE V" << (int)b3 << " 0x" << (int)b2 << (int)b1 << std::endl;
}

void Disassembler::SkneVX0xNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "SKNE V" << (int)b3 << " 0x" << (int)b2 << (int)b1 << std::endl;
}

void Disassembler::SkeVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "SKE V" << (int)b3 << " V" << (int)b2 << std::endl;
}

void Disassembler::LdVX0xNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "LD V" << (int)b3 << " 0x" << (int)b2 << (int)b1 << std::endl;
}

void Disassembler::AddVX0xNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "ADD V" << (int)b3 << " 0x" << (int)b2 << (int)b1 << std::endl;
}

void Disassembler::LdVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "LD V" << (int)b3 << " V" << (int)b2 << std::endl;
}

void Disassembler::OrVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "OR V" << (int)b3 << " V" << (int)b2 << std::endl;
}

void Disassembler::AndVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "AND V" << (int)b3 << " V" << (int)b2 << std::endl;
}

void Disassembler::XorVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "XOR V" << (int)b3 << " V" << (int)b2 << std::endl;
}

void Disassembler::AddVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "ADD V" << (int)b3 << " V" << (int)b2 << std::endl;
}

void Disassembler::SubVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "SUB V" << (int)b3 << " V" << (int)b2 << std::endl;
}

void Disassembler::ShrVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "SHR V" << (int)b3 << std::endl;
}

void Disassembler::SubnVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "SUBN V" << (int)b3 << " V" << (int)b2 << std::endl;
}

void Disassembler::ShlVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "SHL V" << (int)b3 << std::endl;
}

void Disassembler::SkneVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "SKNE V" << (int)b3 << " V" << (int)b2 << std::endl;
}

void Disassembler::LdI0xNNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "LD I 0x" << std::hex << (int)b3 << (int)b2 << (int)b1 << std::endl;
}

void Disassembler::JmpV00xNNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "JMP V0 0x" << std::hex << (int)b3 << (int)b2 << (int)b1 << std::endl;
}

void Disassembler::RndVX0xNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "RND V" << (int)b3 << " 0x" << (int)b2 << (int)b1 << std::endl;
}

void Disassembler::DrwVXVY0xN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "DRW V" << (int)b3 << " V" << (int)b2 << " 0x" << (int)b1 << std::endl;
}

void Disassembler::SkpVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "SKP V" << (int)b3 << std::endl;
}

void Disassembler::SknpVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "SKNP V" << (int)b3 << std::endl;
}

void Disassembler::LdVXDT(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "LD V" << (int)b3 << " DT" << std::endl;
}

void Disassembler::LdVXK(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "LD V" << (int)b3 << " K" << std::endl;
}

void Disassembler::LdDTVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "LD DT V" << (int)b3 << std::endl;
}

void Disassembler::LdSTVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "LD ST V" << (int)b3 << std::endl;
}

void Disassembler::AddIVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "ADD I V" << (int)b3 << std::endl;
}

void Disassembler::LdNVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "LD N V" << (int)b3 << std::endl;
}

void Disassembler::LdDVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "LD D V" << (int)b3 << std::endl;
}

void Disassembler::LdMVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "LD M V" << (int)b3 << std::endl;
}

void Disassembler::LdVXM(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3)
{
    out << "LD V" << (int)b3 << " M" << std::endl;
}
