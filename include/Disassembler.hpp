#ifndef DISASSEMBLER_HPP
#define DISASSEMBLER_HPP

#include <array>
#include <functional>
#include <ostream>
#include <string>

class Disassembler
{
    public:
        Disassembler();
        virtual ~Disassembler();
        bool LoadAssembly(const std::string& path);
        void Disassemble(std::ostream& out);
    private:
        std::array<std::function<void(Disassembler*, std::ostream&, uint8_t, uint8_t, uint8_t)>, 35> mapper;
        void Sys0xNNN(std::ostream& out, uint8_t b1, uint8_t b2, uint8_t b3);
};

#endif // DISASSEMBLER_HPP
