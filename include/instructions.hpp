#pragma once

#include <string>
#include <iostream>
#include <iomanip>

#include "fmt.hpp"
#include "registers.hpp"

enum OpCode
{
    HALT = 0x00,
    CALL_LIT,
    RETURN,

    PUSH_LIT,
    PUSH_REG,
    POP_REG,

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

    INC_REG,
    INC_MEM,

    DEC_REG,
    DEC_MEM,

    JMP_NE,
    JMP_EQ,
    JMP_GT,
    JMP_LT,
    JMP_GE,
    JMP_LE,
    JMP,
};

/*

Example program:

label addresses:
    main: 0x00
    func: 0x10

main:
    MOV $0xffffffff R0
    CALL func
    EXIT

func:
    MOV $0x01, R1
    MOV $0x02, R2
    ADD R1, R2
    MOV R2, R0
    RETURN

*/

std::string to_string(OpCode op);
