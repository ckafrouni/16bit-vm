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
    IP,
};

struct RegisterFile
{
    uint32_t registers[5]{
        0x00000000, // R0
        0x00000000, // R1
        0x00000000, // R2
        0x00000000, // R3
        0x00000000, // IP
    };

    uint32_t &operator[](Register reg);

    std::string to_string();
    std::string to_string(Register modified_register);

    void inspect();
    void inspect(Register modified_register);
};

std::string to_string(Register reg);
