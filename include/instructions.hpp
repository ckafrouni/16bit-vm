#pragma once

#include <string>
#include <iostream>
#include <iomanip>

#include "registers.hpp"

namespace Instructions
{

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

    inline std::string to_string(OpCode op)
    {
        switch (op)
        {
        case OpCode::HALT:
            return "HALT";

        case OpCode::RETURN:
            return "RETURN";
        case OpCode::CALL_LIT:
            return "CALL_LIT";

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

        case OpCode::STORE_LIT_MEM:
            return "STORE_LIT_MEM";
        case OpCode::STORE_REG_MEM:
            return "STORE_REG_MEM";
        case OpCode::STORE_MEM_MEM:
            return "STORE_MEM_MEM";

        case OpCode::LOAD_MEM_REG:
            return "LOAD_MEM_REG";

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
