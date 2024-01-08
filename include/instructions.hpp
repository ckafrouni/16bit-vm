#pragma once

#include <string>
#include <iostream>
#include <iomanip>

#include "registers.hpp"

namespace instructions
{

    typedef uint32_t Addr;

    enum class OpCode
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
        ADD_REG_REG_REG,

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

    enum class MnemonicGroup
    {
        HALT,

        PUSH,
        POP,

        MOV,

        ADD,
        SUB,

        INC,
        DEC,

        JMP,
        CALL,
        RETURN,
    };

    enum class OperandType
    {
        NONE,
        LITERAL,
        REGISTER,
        MEMORY,
        ADDRESS,
    };

    struct OperandProperties
    {
        char *mnemonic;
        int size;
    };

    constexpr OperandProperties getOperandProperties(OperandType type)
    {
        switch (type)
        {
        case OperandType::NONE:
            return {"NONE", 0};
        case OperandType::LITERAL:
            return {"LITERAL", 4};
        case OperandType::REGISTER:
            return {"REGISTER", 1};
        case OperandType::MEMORY:
            return {"MEMORY", 4};
        case OperandType::ADDRESS:
            return {"ADDRESS", 4};
        default:
            return {};
        }
    };

    struct InstructionProperties
    {
        int byte_size;
        MnemonicGroup group;
        char *mnemonic;
        int num_operands;
        std::array<OperandType, 3> operand_types;
    };

    constexpr InstructionProperties getInstructionProperties(OpCode op)
    {
        switch (op)
        {
        case OpCode::HALT:
            return {1, MnemonicGroup::HALT, "HALT", 0, {}};

        case OpCode::PUSH_LIT:
            return {5, MnemonicGroup::PUSH, "PUSH_LIT", 1, {OperandType::LITERAL}};
        case OpCode::PUSH_REG:
            return {2, MnemonicGroup::PUSH, "PUSH_REG", 1, {OperandType::REGISTER}};
        case OpCode::POP_REG:
            return {2, MnemonicGroup::POP, "POP_REG", 1, {OperandType::REGISTER}};
            // case OpCode::POP_MEM: return {2, MnemonicGroup::POP, "POP_MEM", 1, {OperandType::MEMORY}}

        case OpCode::MOV_LIT_REG:
            return {6, MnemonicGroup::MOV, "MOV_LIT_REG", 2, {OperandType::LITERAL, OperandType::REGISTER}};
        case OpCode::MOV_REG_REG:
            return {3, MnemonicGroup::MOV, "MOV_REG_REG", 2, {OperandType::REGISTER, OperandType::REGISTER}};
        case OpCode::MOV_MEM_REG:
            return {3, MnemonicGroup::MOV, "MOV_MEM_REG", 2, {OperandType::MEMORY, OperandType::REGISTER}};

        case OpCode::MOV_LIT_MEM:
            return {6, MnemonicGroup::MOV, "MOV_LIT_MEM", 2, {OperandType::LITERAL, OperandType::MEMORY}};
        case OpCode::MOV_REG_MEM:
            return {3, MnemonicGroup::MOV, "MOV_REG_MEM", 2, {OperandType::REGISTER, OperandType::MEMORY}};
        case OpCode::MOV_MEM_MEM:
            return {3, MnemonicGroup::MOV, "MOV_MEM_MEM", 2, {OperandType::MEMORY, OperandType::MEMORY}};

        case OpCode::ADD_LIT_REG:
            return {6, MnemonicGroup::ADD, "ADD_LIT_REG", 2, {OperandType::LITERAL, OperandType::REGISTER}};
        case OpCode::ADD_REG_REG:
            return {3, MnemonicGroup::ADD, "ADD_REG_REG", 2, {OperandType::REGISTER, OperandType::REGISTER}};
        case OpCode::ADD_REG_REG_REG:
            return {4, MnemonicGroup::ADD, "ADD_REG_REG_REG", 3, {OperandType::REGISTER, OperandType::REGISTER, OperandType::REGISTER}};

        case OpCode::SUB_LIT_REG:
            return {6, MnemonicGroup::SUB, "SUB_LIT_REG", 2, {OperandType::LITERAL, OperandType::REGISTER}};
        case OpCode::SUB_REG_REG:
            return {3, MnemonicGroup::SUB, "SUB_REG_REG", 2, {OperandType::REGISTER, OperandType::REGISTER}};

        case OpCode::INC_REG:
            return {2, MnemonicGroup::INC, "INC_REG", 1, {OperandType::REGISTER}};
        case OpCode::INC_MEM:
            return {2, MnemonicGroup::INC, "INC_MEM", 1, {OperandType::MEMORY}};
        case OpCode::DEC_REG:
            return {2, MnemonicGroup::DEC, "DEC_REG", 1, {OperandType::REGISTER}};
        case OpCode::DEC_MEM:
            return {2, MnemonicGroup::DEC, "DEC_MEM", 1, {OperandType::MEMORY}};

        case OpCode::JMP_NE:
            return {6, MnemonicGroup::JMP, "JMP_NE", 2, {OperandType::LITERAL, OperandType::ADDRESS}};
        case OpCode::JMP_EQ:
            return {6, MnemonicGroup::JMP, "JMP_EQ", 2, {OperandType::LITERAL, OperandType::ADDRESS}};
        case OpCode::JMP_GT:
            return {6, MnemonicGroup::JMP, "JMP_GT", 2, {OperandType::LITERAL, OperandType::ADDRESS}};
        case OpCode::JMP_LT:
            return {6, MnemonicGroup::JMP, "JMP_LT", 2, {OperandType::LITERAL, OperandType::ADDRESS}};
        case OpCode::JMP_GE:
            return {6, MnemonicGroup::JMP, "JMP_GE", 2, {OperandType::LITERAL, OperandType::ADDRESS}};
        case OpCode::JMP_LE:
            return {6, MnemonicGroup::JMP, "JMP_LE", 2, {OperandType::LITERAL, OperandType::ADDRESS}};
        case OpCode::JMP:
            return {5, MnemonicGroup::JMP, "JMP", 1, {OperandType::ADDRESS}};

        case OpCode::CALL:
            return {5, MnemonicGroup::CALL, "CALL", 1, {OperandType::ADDRESS}};
        case OpCode::RETURN:
            return {1, MnemonicGroup::RETURN, "RETURN", 0, {}};

        default:
            return {};
        }
    }

    constexpr char *toString(OpCode op) { return getInstructionProperties(op).mnemonic; }
    constexpr char *toString(MnemonicGroup group)
    {
        switch (group)
        {
        case MnemonicGroup::HALT:
            return "HALT";
        case MnemonicGroup::PUSH:
            return "PUSH";
        case MnemonicGroup::POP:
            return "POP";
        case MnemonicGroup::MOV:
            return "MOV";
        case MnemonicGroup::ADD:
            return "ADD";
        case MnemonicGroup::SUB:
            return "SUB";
        case MnemonicGroup::INC:
            return "INC";
        case MnemonicGroup::DEC:
            return "DEC";
        case MnemonicGroup::JMP:
            return "JMP";
        case MnemonicGroup::CALL:
            return "CALL";
        case MnemonicGroup::RETURN:
            return "RETURN";
        default:
            return "UNKNOWN";
        }
    }
}
