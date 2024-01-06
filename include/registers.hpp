#pragma once

#include <cstdint>
#include <string>

enum Register
{
    // General purpose registers
    R0 = 0x00,
    R1,
    R2,
    R3,

    // Special registers
    ACC, // Accumulator
    IP,  // Instruction Pointer
    SP,  // Stack Pointer
    FP,  // Frame Pointer
};

struct RegisterFile
{
    uint32_t registers[8];

    uint32_t &operator[](Register reg);

    std::string to_string();
    std::string to_string(Register modified_register);

    void inspect();
    void inspect(Register modified_register);
};

std::string to_string(Register reg);
Register to_register(std::string reg);
