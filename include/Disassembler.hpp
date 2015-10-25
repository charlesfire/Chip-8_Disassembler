#ifndef DISASSEMBLER_HPP
#define DISASSEMBLER_HPP

#include <array>
#include <functional>
#include <ostream>
#include <string>
#include <vector>

class Disassembler
{
    public:
        Disassembler();
        virtual ~Disassembler();
        bool LoadAssembly(const std::string& path);
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

        void Sys0xNNN(std::ostream& out, uint8_t b1, uint8_t b2, uint8_t b3);
};

#endif // DISASSEMBLER_HPP
