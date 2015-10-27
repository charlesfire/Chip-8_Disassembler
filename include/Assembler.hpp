#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <array>
#include <functional>
#include <ostream>
#include <regex>
#include <string>
#include <vector>

class Assembler
{
    public:
        Assembler();
        virtual ~Assembler() = default;
        bool LoadCode(std::istream& in);
        bool Assemble(std::ostream& out);
    private:
        std::vector<std::string> lines;
        std::array<std::function<void(Assembler*, std::ostream&, const std::smatch&)>, 35> mapper;
        std::array<std::regex, 35> validInstructions;

        void Sys0xNNN(std::ostream& out, const std::smatch& results);
        void Clear(std::ostream& out, const std::smatch& results);
        void Ret(std::ostream& out, const std::smatch& results);
        void Jmp0xNNN(std::ostream& out, const std::smatch& results);
        void Call0xNNN(std::ostream& out, const std::smatch& results);
        void SkeVX0xNN(std::ostream& out, const std::smatch& results);
        void SkneVX0xNN(std::ostream& out, const std::smatch& results);
        void SkeVXVY(std::ostream& out, const std::smatch& results);
        void LdVX0xNN(std::ostream& out, const std::smatch& results);
        void AddVX0xNN(std::ostream& out, const std::smatch& results);
        void LdVXVY(std::ostream& out, const std::smatch& results);
        void OrVXVY(std::ostream& out, const std::smatch& results);
        void AndVXVY(std::ostream& out, const std::smatch& results);
        void XorVXVY(std::ostream& out, const std::smatch& results);
        void AddVXVY(std::ostream& out, const std::smatch& results);
        void SubVXVY(std::ostream& out, const std::smatch& results);
        void ShrVX(std::ostream& out, const std::smatch& results);
        void SubnVXVY(std::ostream& out, const std::smatch& results);
        void ShlVX(std::ostream& out, const std::smatch& results);
        void SkneVXVY(std::ostream& out, const std::smatch& results);
        void LdI0xNNN(std::ostream& out, const std::smatch& results);
        void JmpV00xNNN(std::ostream& out, const std::smatch& results);
        void RndVX0xNN(std::ostream& out, const std::smatch& results);
        void DrwVXVY0xN(std::ostream& out, const std::smatch& results);
        void SkpVX(std::ostream& out, const std::smatch& results);
        void SknpVX(std::ostream& out, const std::smatch& results);
        void LdVXDT(std::ostream& out, const std::smatch& results);
        void LdVXK(std::ostream& out, const std::smatch& results);
        void LdDTVX(std::ostream& out, const std::smatch& results);
        void LdSTVX(std::ostream& out, const std::smatch& results);
        void AddIVX(std::ostream& out, const std::smatch& results);
        void LdNVX(std::ostream& out, const std::smatch& results);
        void LdDVX(std::ostream& out, const std::smatch& results);
        void LdMVX(std::ostream& out, const std::smatch& results);
        void LdVXM(std::ostream& out, const std::smatch& results);
};

#endif // ASSEMBLER_HPP
