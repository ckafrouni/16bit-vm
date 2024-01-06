#pragma once

#include <cstdint>

#include "hexutils.hpp"
#include "instructions.hpp"
#include "memory.hpp"
#include "registers.hpp"

struct Interpreter
{
    Memory &memory;
    RegisterFile &registers;
    Register modified_register;
    bool running;

    uint32_t run(Memory *program, uint32_t entry_point);
    bool step(Memory *program);
};
