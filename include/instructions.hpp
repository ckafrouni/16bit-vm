#pragma once

#include <string>
#include <iostream>
#include <iomanip>

#include "fmt.hpp"
#include "registers.hpp"

enum OpCode
{
    RETURN = 0x00,

    MOV_LIT_REG,
    MOV_REG_REG,

    STORE_LIT_MEM,
    STORE_REG_MEM,
    STORE_MEM_MEM,

    LOAD_MEM_REG,

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

std::string to_string(OpCode op);
