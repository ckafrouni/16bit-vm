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

        inline Addr encode_mov_lit_reg(Addr addr, uint32_t lit, uint8_t reg)
        {
            this->program->write8(this->current_addr, MOV_LIT_REG);
            this->program->write32(this->current_addr + 1, lit);
            this->program->write8(this->current_addr + 5, reg);
            return 6; // Opcode + Value + Reg
        }

        inline Addr encode_mov_reg_reg(Addr addr, uint8_t reg1, uint8_t reg2)
        {
            this->program->write8(this->current_addr, MOV_REG_REG);
            this->program->write8(this->current_addr + 1, reg1);
            this->program->write8(this->current_addr + 2, reg2);
            return 3; // Opcode + Reg + Reg
        }

    public:
        Compiler() : program(new memory::Memory(initial_size)) {}
        ~Compiler() { delete this->program; }

        void assemble(std::string source);

        inline memory::Memory *get_program() { return this->program; }
    };

    void encode_lit_reg(Compiler *compiler, std::string source);
}
