#include <iostream>
#include <iomanip>
#include <string>

#include "interpreter.hpp"

#include "utils/fmt.hpp"
#include "utils/hexutils.hpp"

#include "instructions.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "addr.hpp"

void push(Memory *memory, RegisterFile *registers, uint32_t val)
{
    (*registers)[SP] -= 4;
    memory->write32((*registers)[SP], val);
    (*registers)[IP] += 5;
}

uint32_t pop(Memory *memory, RegisterFile *registers)
{
    auto val = memory->read32((*registers)[(Register)SP]);
    (*registers)[(Register)SP] += 4;
    return val;
}

bool Interpreter::step(Memory *program)
{
    using namespace Instructions;

    auto op = program->read8(registers[IP]);
    std::cout << utils::colorize("## Executing: " + to_string((OpCode)op), utils::FG_RED, utils::BOLD) << std::endl;

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
    case PUSH_LIT:
    {
        auto lit = program->read32(registers[IP] + 1);
        push(&memory, &registers, lit);
        modified_register = SP;
        break;
    }
    case PUSH_REG:
    {
        auto reg = program->read8(registers[IP] + 1);
        push(&memory, &registers, registers[(Register)reg]);
        modified_register = SP;
        break;
    }
    case POP_REG:
    {
        auto reg = program->read8(registers[IP] + 1);
        registers[(Register)reg] = pop(&memory, &registers);
        modified_register = (Register)reg;
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

    case CALL_LIT:
    {
        Addr addr = program->read32(registers[IP] + 1);
        push(&memory, &registers, registers[IP] + 5); // return address
        // push registers
        push(&memory, &registers, registers[R0]);
        push(&memory, &registers, registers[R1]);
        push(&memory, &registers, registers[R2]);
        push(&memory, &registers, registers[R3]);
        push(&memory, &registers, registers[ACC]);

        registers[FP] = registers[SP];
        registers[IP] = addr;
        modified_register = IP;
        break;
    }
    default:
    {
        std::cout << utils::colorize("Unknown opcode: " + to_string((OpCode)op), utils::FG_RED, utils::BOLD) << std::endl;
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

    std::cout << colorize("## START OF PROGRAM", utils::Colors::FG_RED, utils::Styles::BOLD) << std::endl;

    while (step(program))
    {
        registers.inspect(modified_register);
    }
    auto ret = registers[R0];

    std::cout << colorize("## END OF PROGRAM", utils::Colors::FG_RED, utils::Styles::BOLD) + " "
              << colorize("R0 = " + utils::hexstr32(ret), utils::Colors::FG_WHITE, utils::Styles::BOLD) << std::endl;

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
        std::cout << utils::colorize("## BREAK (Press a key to start)", utils::Colors::FG_YELLOW, utils::Styles::BOLD);
        std::cin.get();

        while (step(program))
        {
            registers.inspect(modified_register);
            std::cout << utils::colorize("## BREAK (Press a key to continue)", utils::Colors::FG_YELLOW, utils::Styles::BOLD);
            std::cin.get();
        }
        auto ret = registers[R0];

        std::cout << colorize("## END OF PROGRAM", utils::Colors::FG_RED, utils::Styles::BOLD) + " "
                  << colorize("R0 = " + utils::hexstr32(ret), utils::Colors::FG_WHITE, utils::Styles::BOLD) << std::endl;

        return registers[R0];
    }
}
