#pragma once

#include <string>
#include <cstdint>
#include <map>

#include "memory.hpp"

namespace compiler
{
    class Compiler
    {
    private:
        memory::Memory *program;
        std::map<std::string, Addr> labels;
        std::map<Addr, std::string> unresolved_labels;
        Addr current_addr = 0x00;

    public:
        Compiler()
        {
            this->program = new memory::Memory(1024);
        }

        ~Compiler()
        {
            delete this->program;
        }

        void compile(std::string source);

        inline memory::Memory *get_program() { return this->program; }
    };
}
