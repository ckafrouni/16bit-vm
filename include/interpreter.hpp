#pragma once

#include <cstdint>

#include "instructions.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "addr.hpp"

enum Mode
{
    DEBUG,
    RELEASE,
};

struct Interpreter
{
    Memory &memory;
    RegisterFile &registers;
    Register modified_register;
    bool running;

    uint32_t run(Memory *program, Addr entry_point);
    uint32_t run(Memory *program, Addr entry_point, Mode mode);
    bool step(Memory *program);
};
