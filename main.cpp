#include <iostream>
#include <fstream>
#include "Disassembler.hpp"

int main(int argc, const char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Wrong number of parameters. Should be 2." << std::endl;
        return 0;
    }

    Disassembler disassembler;
    if (disassembler.LoadAssembly(argv[2]))
        disassembler.Disassemble(std::cout);

    return 0;
}
