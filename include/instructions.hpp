#pragma once

#include <string>
#include <iostream>
#include <iomanip>

#include "colors.hpp"

enum OpCode
{
    RETURN = 0x00,

    MOV_LIT_REG,
    MOV_REG_REG,

    ADD_LIT_REG,
    ADD_REG_REG,

    SUB_LIT_REG,
    SUB_REG_REG,

    JMP_NE,
    JMP_EQ,
    JMP_GT,
    JMP_LT,
    JMP_GE,
    JMP_LE,
    JMP,

    CMP_LIT_REG,
    CMP_REG_REG,
};

enum Register
{
    R0 = 0x00,
    R1,
    R2,
    R3,
    IP,
};

struct RegisterValue
{
    Register reg;
    unsigned short value;
};

struct Instruction
{
    OpCode op;
    unsigned short args[2];
};

struct Program
{
    Instruction *instructions;
    unsigned int instruction_count;
};

struct Interpreter
{
    RegisterValue registers[4]{
        RegisterValue{R0, 0x0000},
        RegisterValue{R1, 0x0000},
        RegisterValue{R2, 0x0000},
        RegisterValue{R3, 0x0000},
    };

    RegisterValue ip = {IP, 0x0000};
    bool zf = false;
    bool cf = false;

    unsigned short run(Program program);
    void inspect();
    void inspect(Register changed_register);
};
