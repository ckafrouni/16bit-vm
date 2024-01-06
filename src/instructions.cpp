#include <string>

#include "instructions.hpp"

std::string to_string(OpCode op)
{
    switch (op)
    {
    case OpCode::HALT:
        return "HALT";
    case OpCode::RETURN:
        return "RETURN";
    case OpCode::CALL_LIT:
        return "CALL_LIT";
    case OpCode::CALL_REG:
        return "CALL_REG";
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
    case OpCode::INC_REG:
        return "INC_REG";
    case OpCode::INC_MEM:
        return "INC_MEM";
    case OpCode::DEC_REG:
        return "DEC_REG";
    case OpCode::DEC_MEM:
        return "DEC_MEM";
    case OpCode::SUB_LIT_REG:
        return "SUB_LIT_REG";
    case OpCode::SUB_REG_REG:
        return "SUB_REG_REG";
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
