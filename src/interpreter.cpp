#include <iostream>
#include <iomanip>
#include <string>

#include "interpreter.hpp"

#include "fmt.hpp"
#include "instructions.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "hexutils.hpp"

bool Interpreter::step(Memory *program)
{
    auto op = program->read8(registers[IP]);
    std::cout << "OP: " << to_string((OpCode)op) << std::endl;

    switch (op)
    {
    case HALT:
    {
        running = false;
        registers[IP] += 1;
        modified_register = IP;
        break;
    }
    case RETURN:
    {
        // TODO after implementing the stack and CALL
        break;
    }
    case MOV_LIT_REG:
    {
        auto lit = program->read32(registers[IP] + 1);
        auto reg = program->read8(registers[IP] + 5);
        std::cout << "MOV_LIT_REG $" << hexstr32(lit) << ", " << to_string((Register)reg) << std::endl;
        modified_register = (Register)reg;
        registers[modified_register] = lit;
        registers[IP] += 6;
        break;
    }
    case MOV_REG_REG:
    {
        auto src = program->read8(registers[IP] + 1);
        auto dst = program->read8(registers[IP] + 2);
        modified_register = (Register)dst;
        registers[modified_register] = registers[(Register)src];
        registers[IP] += 3;
        break;
    }
    case STORE_LIT_MEM:
    {
        auto lit = program->read32(registers[IP] + 1);
        auto addr = program->read32(registers[IP] + 5);
        memory.write32(addr, lit);
        registers[IP] += 9;
        modified_register = IP;
        break;
    }
    case STORE_REG_MEM:
    {
        auto reg = program->read8(registers[IP] + 1);
        auto addr = program->read32(registers[IP] + 2);
        memory.write32(addr, registers[(Register)reg]);
        registers[IP] += 6;
        modified_register = IP;
        break;
    }
    case STORE_MEM_MEM:
    {
        auto src_addr = program->read32(registers[IP] + 1);
        auto dst_addr = program->read32(registers[IP] + 5);
        auto val = memory.read32(src_addr);
        memory.write32(dst_addr, val);
        registers[IP] += 9;
        modified_register = IP;
        break;
    }
    case LOAD_MEM_REG:
    {
        auto addr = program->read32(registers[IP] + 1);
        auto reg = program->read8(registers[IP] + 5);
        modified_register = (Register)reg;
        registers[modified_register] = memory.read32(addr);
        registers[IP] += 6;
        break;
    }
    case ADD_LIT_REG:
    {
        auto lit = program->read32(registers[IP] + 1);
        auto reg = program->read8(registers[IP] + 5);
        modified_register = (Register)reg;
        registers[modified_register] += lit;
        registers[IP] += 6;
        break;
    }
    case ADD_REG_REG:
    {
        auto src = program->read8(registers[IP] + 1);
        auto dst = program->read8(registers[IP] + 2);
        modified_register = (Register)dst;
        registers[modified_register] += registers[(Register)src];
        registers[IP] += 3;
        break;
    }
    case SUB_LIT_REG:
    {
        auto lit = program->read32(registers[IP] + 1);
        auto reg = program->read8(registers[IP] + 5);
        modified_register = (Register)reg;
        registers[modified_register] -= lit;
        registers[IP] += 6;
        break;
    }
    case SUB_REG_REG:
    {
        auto src = program->read8(registers[IP] + 1);
        auto dst = program->read8(registers[IP] + 2);
        modified_register = (Register)dst;
        registers[modified_register] -= registers[(Register)src];
        registers[IP] += 3;
        break;
    }
    case INC_REG:
    {
        auto reg = program->read8(registers[IP] + 1);
        modified_register = (Register)reg;
        registers[modified_register] += 1;
        registers[IP] += 2;
        break;
    }
    case INC_MEM:
    {
        auto addr = program->read32(registers[IP] + 1);
        auto val = memory.read32(addr);
        memory.write32(addr, val + 1);
        registers[IP] += 5;
        modified_register = IP;
        break;
    }
    case DEC_REG:
    {
        auto reg = program->read8(registers[IP] + 1);
        modified_register = (Register)reg;
        registers[modified_register] -= 1;
        registers[IP] += 2;
        break;
    }
    case DEC_MEM:
    {
        auto addr = program->read32(registers[IP] + 1);
        auto val = memory.read32(addr);
        memory.write32(addr, val - 1);
        registers[IP] += 5;
        modified_register = IP;
        break;
    }
    case JMP:
    {
        auto addr = program->read32(registers[IP] + 1);
        registers[IP] = addr;
        modified_register = IP;
        break;
    }
    case JMP_NE: // JMP_NE Rx <addr>
    {
        auto reg = program->read8(registers[IP] + 1);
        auto addr = program->read32(registers[IP] + 2);
        if (registers[ACC] != registers[(Register)reg])
        {
            registers[IP] = addr;
        }
        else
        {
            registers[IP] += 6;
        }
        modified_register = IP;
        break;
    }
    case JMP_EQ: // JMP_EQ Rx <addr>
    {
        auto reg = program->read8(registers[IP] + 1);
        auto addr = program->read32(registers[IP] + 2);
        if (registers[ACC] == registers[(Register)reg])
        {
            registers[IP] = addr;
        }
        else
        {
            registers[IP] += 6;
        }
        modified_register = IP;
        break;
    }
    case JMP_GT: // JMP_GT Rx <addr>
    {
        auto reg = program->read8(registers[IP] + 1);
        auto addr = program->read32(registers[IP] + 2);
        if (registers[ACC] > registers[(Register)reg])
        {
            registers[IP] = addr;
        }
        else
        {
            registers[IP] += 6;
        }
        modified_register = IP;
        break;
    }
    case JMP_LT: // JMP_LT Rx <addr>
    {
        auto reg = program->read8(registers[IP] + 1);
        auto addr = program->read32(registers[IP] + 2);
        if (registers[ACC] < registers[(Register)reg])
        {
            registers[IP] = addr;
        }
        else
        {
            registers[IP] += 6;
        }
        modified_register = IP;
        break;
    }
    default:
    {
        std::cout << "Unknown opcode: " << to_string((OpCode)op) << std::endl;
        running = false;
        registers[IP] += 1;
        modified_register = IP;
        break;
    }
    }

    return running;
}

uint32_t Interpreter::run(Memory *program, uint32_t entry_point)
{
    registers[IP] = entry_point;
    running = true;

    registers.inspect(modified_register);
    std::cout << fmt::colorize("Press enter to continue...", fmt::Colors::FG_GREEN);
    std::cin.get();

    while (step(program))
    {
        registers.inspect(modified_register);
        std::cout << fmt::colorize("Press enter to continue...", fmt::Colors::FG_GREEN);
        std::cin.get();
    }

    std::cout << "Program finished." << std::endl;

    return registers[R0];
}
