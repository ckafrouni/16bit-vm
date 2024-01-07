#pragma once

#include <string>
#include <iostream>
#include <iomanip>

#include "registers.hpp"

namespace instructions
{

    enum OpCode
    {
        HALT = 0x00,

        // Stack
        PUSH_LIT,
        PUSH_REG,
        POP_REG,

        // Register
        MOV_LIT_REG,
        MOV_REG_REG,
        MOV_MEM_REG,

        // Memory
        MOV_LIT_MEM,
        MOV_REG_MEM,
        MOV_MEM_MEM,

        // Arithmetic
        ADD_LIT_REG,
        ADD_REG_REG,

        SUB_LIT_REG,
        SUB_REG_REG,

        INC_REG,
        INC_MEM,

        DEC_REG,
        DEC_MEM,

        // Jump
        JMP_NE,
        JMP_EQ,
        JMP_GT,
        JMP_LT,
        JMP_GE,
        JMP_LE,
        JMP,

        // Subroutine
        CALL,
        RETURN,

    };

    inline std::string to_string(OpCode op)
    {
        switch (op)
        {
        case OpCode::HALT:
            return "HALT";

        case OpCode::RETURN:
            return "RETURN";
        case OpCode::CALL:
            return "CALL";

        case OpCode::PUSH_LIT:
            return "PUSH_LIT";
        case OpCode::PUSH_REG:
            return "PUSH_REG";
        case OpCode::POP_REG:
            return "POP_REG";

        case OpCode::MOV_LIT_REG:
            return "MOV_LIT_REG";
        case OpCode::MOV_REG_REG:
            return "MOV_REG_REG";

        case OpCode::MOV_LIT_MEM:
            return "MOV_LIT_MEM";
        case OpCode::MOV_REG_MEM:
            return "MOV_REG_MEM";
        case OpCode::MOV_MEM_MEM:
            return "MOV_MEM_MEM";

        case OpCode::MOV_MEM_REG:
            return "MOV_MEM_REG";

        case OpCode::ADD_LIT_REG:
            return "ADD_LIT_REG";
        case OpCode::ADD_REG_REG:
            return "ADD_REG_REG";

        case OpCode::SUB_LIT_REG:
            return "SUB_LIT_REG";
        case OpCode::SUB_REG_REG:
            return "SUB_REG_REG";

        case OpCode::INC_REG:
            return "INC_REG";
        case OpCode::INC_MEM:
            return "INC_MEM";
        case OpCode::DEC_REG:
            return "DEC_REG";
        case OpCode::DEC_MEM:
            return "DEC_MEM";

        case OpCode::JMP_NE:
            return "JMP_NE";
        case OpCode::JMP_EQ:
            return "JMP_EQ";
        case OpCode::JMP_GT:
            return "JMP_GT";
        case OpCode::JMP_LT:
            return "JMP_LT";
        case OpCode::JMP_GE:
            return "JMP_GE";
        case OpCode::JMP_LE:
            return "JMP_LE";
        case OpCode::JMP:
            return "JMP";

        default:
            return "UNKNOWN";
        }
    }

}
