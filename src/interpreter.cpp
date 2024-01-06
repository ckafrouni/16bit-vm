#include <iostream>
#include <iomanip>
#include <string>

#include "interpreter.hpp"

#include "fmt.hpp"
#include "instructions.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "hexutils.hpp"

uint32_t Interpreter::run(Memory *program, uint32_t entry_point)
{
    this->registers[IP] = entry_point;
    auto running = true;

    auto modified_register = IP;
    this->registers.inspect(modified_register);
    std::cout << "Press enter to continue...";
    std::cin.get();

    while (running)
    {
        auto op = program->read8(this->registers[IP]);
        std::cout << "OP: " << to_string((OpCode)op) << std::endl;
        switch (op)
        {
        case OpCode::RETURN:
        {
            running = false;
            this->registers[IP] += 1;
            modified_register = IP;
            break;
        }
        case OpCode::MOV_LIT_REG:
        {
            auto lit = program->read32(this->registers[IP] + 1);
            auto reg = program->read8(this->registers[IP] + 5);
            std::cout << "MOV_LIT_REG $" << hexstr32(lit) << ", " << to_string((Register)reg) << std::endl;
            modified_register = (Register)reg;
            registers[modified_register] = lit;
            this->registers[IP] += 6;
            break;
        }
        case OpCode::MOV_REG_REG:
        {
            auto src = program->read8(this->registers[IP] + 1);
            auto dst = program->read8(this->registers[IP] + 2);
            modified_register = (Register)dst;
            registers[modified_register] = registers[(Register)src];
            this->registers[IP] += 3;
            break;
        }
        case OpCode::STORE_LIT_MEM:
        {
            auto lit = program->read32(this->registers[IP] + 1);
            auto addr = program->read32(this->registers[IP] + 5);
            memory.write32(addr, lit);
            this->registers[IP] += 9;
            modified_register = IP;
            break;
        }
        case OpCode::STORE_REG_MEM:
        {
            auto reg = program->read8(this->registers[IP] + 1);
            auto addr = program->read32(this->registers[IP] + 2);
            memory.write32(addr, registers[(Register)reg]);
            this->registers[IP] += 6;
            modified_register = IP;
            break;
        }
        case OpCode::STORE_MEM_MEM:
        {
            auto src_addr = program->read32(this->registers[IP] + 1);
            auto dst_addr = program->read32(this->registers[IP] + 5);
            auto val = memory.read32(src_addr);
            memory.write32(dst_addr, val);
            this->registers[IP] += 9;
            modified_register = IP;
            break;
        }
        case OpCode::LOAD_MEM_REG:
        {
            auto addr = program->read32(this->registers[IP] + 1);
            auto reg = program->read8(this->registers[IP] + 5);
            modified_register = (Register)reg;
            registers[modified_register] = memory.read32(addr);
            this->registers[IP] += 6;
            break;
        }
        case OpCode::ADD_LIT_REG:
        {
            auto lit = program->read32(this->registers[IP] + 1);
            auto reg = program->read8(this->registers[IP] + 5);
            modified_register = (Register)reg;
            registers[modified_register] += lit;
            this->registers[IP] += 6;
            break;
        }
        case OpCode::ADD_REG_REG:
        {
            auto src = program->read8(this->registers[IP] + 1);
            auto dst = program->read8(this->registers[IP] + 2);
            modified_register = (Register)dst;
            registers[modified_register] += registers[(Register)src];
            this->registers[IP] += 3;
            break;
        }
        case OpCode::SUB_LIT_REG:
        {
            auto lit = program->read32(this->registers[IP] + 1);
            auto reg = program->read8(this->registers[IP] + 5);
            modified_register = (Register)reg;
            registers[modified_register] -= lit;
            this->registers[IP] += 6;
            break;
        }
        case OpCode::SUB_REG_REG:
        {
            auto src = program->read8(this->registers[IP] + 1);
            auto dst = program->read8(this->registers[IP] + 2);
            modified_register = (Register)dst;
            registers[modified_register] -= registers[(Register)src];
            this->registers[IP] += 3;
            break;
        }
        case OpCode::JMP_NE:
        {
            auto addr = program->read32(this->registers[IP] + 1);
            auto reg = program->read8(this->registers[IP] + 5);
            if (registers[(Register)reg] != 0)
            {
                this->registers[IP] = addr;
            }
            else
            {
                this->registers[IP] += 6;
            }
            modified_register = IP;
            break;
        }
        case OpCode::JMP_EQ:
        {
            auto addr = program->read32(this->registers[IP] + 1);
            auto reg = program->read8(this->registers[IP] + 5);
            if (registers[(Register)reg] == 0)
            {
                this->registers[IP] = addr;
            }
            else
            {
                this->registers[IP] += 6;
            }
            modified_register = IP;
            break;
        }
        case OpCode::JMP:
        {
            auto addr = program->read32(this->registers[IP] + 1);
            this->registers[IP] = addr;
            modified_register = IP;
            break;
        }
            // TODO rest of the instructions
        }

        this->registers.inspect(modified_register);
        std::cout << "Press enter to continue...";
        std::cin.get();
    }

    std::cout << "Program finished." << std::endl;

    return registers[R0];
}
