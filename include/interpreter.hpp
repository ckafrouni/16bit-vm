#pragma once

#include <cstdint>

#include "hexutils.hpp"
#include "instructions.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "addr.hpp"

struct Interpreter
{
    Memory &memory;
    RegisterFile &registers;
    Register modified_register;
    bool running;

    uint32_t run(Memory *program, Addr entry_point);
    bool step(Memory *program);
};
