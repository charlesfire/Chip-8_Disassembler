#ifndef DISASSEMBLER_HPP
#define DISASSEMBLER_HPP

class Disassembler
{
    public:
        Disassembler();
        virtual ~Disassembler();
        bool LoadAssembly(const std::string& path);
        void Disassemble(std::ostream& out);
    private:
};

#endif // DISASSEMBLER_HPP
