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

    uint32_t run(Memory *program, uint32_t entry_point);
};
