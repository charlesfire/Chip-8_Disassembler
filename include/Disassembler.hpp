#ifndef DISASSEMBLER_HPP
#define DISASSEMBLER_HPP

#include <array>
#include <functional>
#include <ostream>
#include <istream>
#include <string>
#include <vector>

class Disassembler
{
    public:
        Disassembler();
        virtual ~Disassembler() = default;
        bool LoadAssembly(std::istream& in);
        bool Disassemble(std::ostream& out);

        struct Opcodes
        {
            static const int NBOPCODE = 35;
            static std::array<uint16_t, NBOPCODE> mask;
            static std::array<int, NBOPCODE> id;
        };
    private:
        std::vector<uint16_t> program;
        std::array<std::function<void(Disassembler*, std::ostream&, uint8_t, uint8_t, uint8_t)>, 35> mapper;

        void Sys0xNNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void Clear(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void Ret(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void Jmp0xNNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void Call0xNNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void SkeVX0xNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void SkneVX0xNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void SkeVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void LdVX0xNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void AddVX0xNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void LdVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void OrVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void AndVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void XorVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void AddVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void SubVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void ShrVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void SubnVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void ShlVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void SkneVXVY(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void LdI0xNNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void JmpV00xNNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void RndVX0xNN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void DrwVXVY0xN(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void SkpVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void SknpVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void LdVXDT(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void LdVXK(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void LdDTVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void LdSTVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void AddIVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void LdNVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void LdDVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void LdMVX(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
        void LdVXM(std::ostream& out, const uint8_t b1, const uint8_t b2, const uint8_t b3);
};

#endif // DISASSEMBLER_HPP
