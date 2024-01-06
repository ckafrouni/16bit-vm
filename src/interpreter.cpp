#include <iostream>
#include <iomanip>
#include <string>

#include "interpreter.hpp"

#include "fmt.hpp"
#include "instructions.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "hexutils.hpp"
#include "addr.hpp"

bool Interpreter::step(Memory *program)
{
    auto op = program->read8(registers[IP]);

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
        Addr addr = program->read32(registers[IP] + 5);
        memory.write32(addr, lit);
        registers[IP] += 9;
        modified_register = IP;
        break;
    }
    case STORE_REG_MEM:
    {
        auto reg = program->read8(registers[IP] + 1);
        Addr addr = program->read32(registers[IP] + 2);
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
        Addr addr = program->read32(registers[IP] + 1);
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
        Addr addr = program->read32(registers[IP] + 1);
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
        Addr addr = program->read32(registers[IP] + 1);
        auto val = memory.read32(addr);
        memory.write32(addr, val - 1);
        registers[IP] += 5;
        modified_register = IP;
        break;
    }
    case JMP:
    {
        Addr addr = program->read32(registers[IP] + 1);
        registers[IP] = addr;
        modified_register = IP;
        break;
    }
    case JMP_NE: // JMP_NE Rx <addr>
    {
        auto reg = program->read8(registers[IP] + 1);
        Addr addr = program->read32(registers[IP] + 2);
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
        Addr addr = program->read32(registers[IP] + 2);
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
        Addr addr = program->read32(registers[IP] + 2);
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
        Addr addr = program->read32(registers[IP] + 2);
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
        std::cout << fmt::colorize("Unknown opcode: " + to_string((OpCode)op), fmt::FG_RED, fmt::BOLD) << std::endl;
        running = false;
        registers[IP] += 1;
        modified_register = IP;
        break;
    }
    }

    return running;
}

uint32_t Interpreter::run(Memory *program, Addr entry_point)
{
    registers[IP] = entry_point;
    running = true;

    std::cout << colorize("## START OF PROGRAM", fmt::Colors::FG_RED, fmt::Styles::BOLD) << std::endl;

    while (step(program))
    {
        registers.inspect(modified_register);
    }
    auto ret = registers[R0];

    std::cout << colorize("## END OF PROGRAM", fmt::Colors::FG_RED, fmt::Styles::BOLD) + " "
              << colorize("R0 = " + hexstr32(ret), fmt::Colors::FG_WHITE, fmt::Styles::BOLD) << std::endl;

    return registers[R0];
}

uint32_t Interpreter::run(Memory *program, Addr entry_point, Mode mode)
{
    if (mode != Mode::DEBUG)
    {
        return run(program, entry_point);
    }
    else
    {
        registers[IP] = entry_point;
        running = true;

        registers.inspect(modified_register);
        std::cout << fmt::colorize("## BREAK (Press a key to start)", fmt::Colors::FG_YELLOW, fmt::Styles::BOLD);
        std::cin.get();

        while (step(program))
        {
            registers.inspect(modified_register);
            std::cout << fmt::colorize("## BREAK (Press a key to continue)", fmt::Colors::FG_YELLOW, fmt::Styles::BOLD);
            std::cin.get();
        }
        auto ret = registers[R0];

        std::cout << colorize("## END OF PROGRAM", fmt::Colors::FG_RED, fmt::Styles::BOLD) + " "
                  << colorize("R0 = " + hexstr32(ret), fmt::Colors::FG_WHITE, fmt::Styles::BOLD) << std::endl;

        return registers[R0];
    }
}
