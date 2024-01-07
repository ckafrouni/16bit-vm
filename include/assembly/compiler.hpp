#pragma once

#include <string>
#include <cstdint>
#include <map>

#include "memory.hpp"

namespace compiler
{
    static const int initial_size = 1024; // 1KB

    class Compiler
    {
    private:
        memory::Memory *program;
        std::map<std::string, Addr> labels;
        std::map<Addr, std::string> unresolved_labels;
        Addr current_addr = 0x00;

    public:
        Compiler() : program(new memory::Memory(initial_size)) {}
        ~Compiler() { delete this->program; }

        void assemble(std::string source);

        inline memory::Memory *get_program() { return this->program; }
    };
}
