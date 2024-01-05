#include <iostream>
#include <iomanip>
#include <string>

#include "colors.hpp"
#include "instructions.hpp"

unsigned short Interpreter::run(Program program)
{
    // for (unsigned short i = 0; i < program.instruction_count; i++)
    while (true)
    {
        Instruction instruction = program.instructions[ip++];
        switch (instruction.op)
        {
            /** MOV */
        case MOV_LIT_REG:
            registers[instruction.args[1]] = instruction.args[0];
            break;
        case MOV_REG_REG:
            registers[instruction.args[1]] = registers[instruction.args[0]];
            break;
            /** ADD */
        case ADD_LIT_REG:
            if (registers[instruction.args[1]] + instruction.args[0] > 0xFFFF)
                cf = true;
            else
                cf = false;
            registers[instruction.args[1]] += instruction.args[0];
            break;
        case ADD_REG_REG:
            if (registers[instruction.args[1]] + registers[instruction.args[0]] > 0xFFFF)
                cf = true;
            else
                cf = false;
            registers[instruction.args[1]] += registers[instruction.args[0]];
            break;
            /** SUB */
        case SUB_LIT_REG:
            if (registers[instruction.args[1]] - instruction.args[0] > 0xFFFF)
                cf = true;
            else
                cf = false;
            registers[instruction.args[1]] -= instruction.args[0];
            break;
        case SUB_REG_REG:
            if (registers[instruction.args[1]] - registers[instruction.args[0]] > 0xFFFF)
                cf = true;
            else
                cf = false;
            registers[instruction.args[1]] -= registers[instruction.args[0]];
            break;
            /** JMP */
        case JMP_NE:
            if (!zf)
                ip = instruction.args[0];
            break;
        case JMP_EQ:
            if (zf)
                ip = instruction.args[0];
            break;
        case JMP_GT:
            if (!zf && !cf)
                ip = instruction.args[0];
            break;
        case JMP_LT:
            if (!zf && cf)
                ip = instruction.args[0];
            break;
        case JMP_GE:
            if (zf || !cf)
                ip = instruction.args[0];
            break;
        case JMP_LE:
            if (zf || cf)
                ip = instruction.args[0];
            break;
        case JMP:
            ip = instruction.args[0];
            break;
            /** CMP */
        case CMP_LIT_REG:
            // Update zero flag and carry flag
            if (registers[instruction.args[1]] == instruction.args[0])
                zf = true;
            else
                zf = false;
            if (registers[instruction.args[1]] < instruction.args[0])
                cf = true;
            else
                cf = false;
            break;
        case CMP_REG_REG:
            // Update zero flag and carry flag
            if (registers[instruction.args[1]] == registers[instruction.args[0]])
                zf = true;
            else
                zf = false;
            if (registers[instruction.args[1]] < registers[instruction.args[0]])
                cf = true;
            else
                cf = false;
            break;

        case RETURN:
            return registers[0];
        }

        inspect((Register)instruction.args[1]);
    }

    return this->registers[0];
}

std::string disassemble(Program program)
{
    std::string result = "";
    for (size_t i = 0; i < program.instruction_count; i++)
    {
        Instruction instruction = program.instructions[i];
        result += "#" + std::to_string(i) + " ";
        switch (instruction.op)
        {
        case MOV_LIT_REG:
            result += "MOV ";
            result += "$" + std::to_string(instruction.args[0]);
            result += " ";
            result += "R" + std::to_string(instruction.args[1]);
            break;
        case MOV_REG_REG:
            result += "MOV ";
            result += "R" + std::to_string(instruction.args[0]);
            result += " ";
            result += "R" + std::to_string(instruction.args[1]);
            break;
        case ADD_LIT_REG:
            result += "ADD ";
            result += "$" + std::to_string(instruction.args[0]);
            result += " ";
            result += "R" + std::to_string(instruction.args[1]);
            break;
        case ADD_REG_REG:
            result += "ADD ";
            result += "R" + std::to_string(instruction.args[0]);
            result += " ";
            result += "R" + std::to_string(instruction.args[1]);
            break;
        case SUB_LIT_REG:
            result += "SUB ";
            result += "$" + std::to_string(instruction.args[0]);
            result += " ";
            result += "R" + std::to_string(instruction.args[1]);
            break;
        case SUB_REG_REG:
            result += "SUB ";
            result += "R" + std::to_string(instruction.args[0]);
            result += " ";
            result += "R" + std::to_string(instruction.args[1]);
            break;
        case JMP_NE:
            result += "JMP_NE ";
            result += std::to_string(instruction.args[0]);
            result += " ";
            result += std::to_string(instruction.args[1]);
            break;
        case JMP_EQ:
            result += "JMP_EQ ";
            result += std::to_string(instruction.args[0]);
            result += " ";
            result += std::to_string(instruction.args[1]);
            break;
        case JMP_GT:
            result += "JMP_GT ";
            result += std::to_string(instruction.args[0]);
            result += " ";
            result += std::to_string(instruction.args[1]);
            break;
        case JMP_LT:
            result += "JMP_LT ";
            result += std::to_string(instruction.args[0]);
            result += " ";
            result += std::to_string(instruction.args[1]);
            break;
        case JMP_GE:
            result += "JMP_GE ";
            result += std::to_string(instruction.args[0]);
            result += " ";
            result += std::to_string(instruction.args[1]);
            break;
        case JMP_LE:
            result += "JMP_LE ";
            result += std::to_string(instruction.args[0]);
            result += " ";
            result += std::to_string(instruction.args[1]);
            break;
        case JMP:
            result += "JMP ";
            result += std::to_string(instruction.args[0]);
            result += " ";
            result += std::to_string(instruction.args[1]);
            break;
        case CMP_LIT_REG:
            result += "CMP ";
            result += "$" + std::to_string(instruction.args[0]);
            result += " ";
            result += "R" + std::to_string(instruction.args[1]);
            break;
        case CMP_REG_REG:
            result += "CMP ";
            result += "R" + std::to_string(instruction.args[0]);
            result += " ";
            result += "R" + std::to_string(instruction.args[1]);
            break;
        case RETURN:
            result += "RETURN ";
            result += std::to_string(instruction.args[0]);
            result += " ";
            result += std::to_string(instruction.args[1]);
            break;
        }
        if (i < program.instruction_count - 1)
        {
            result += "\n";
        }
    }
    return result;
}
