#include <iostream>
#include "instructions.hpp"
#include "utils.hpp"

int main()
{
    Interpreter Interpreter = {.registers = {0xFF, 0xFF, 0xFF, 0xFF}};

    // Inspect
    Interpreter.inspect();

    Program program = {
        new Instruction[]{
            {MOV_LIT_REG, {0x00, Register::R0}}, // 0x00
            {ADD_LIT_REG, {0x01, Register::R0}}, // 0x01
            {CMP_LIT_REG, {0x05, Register::R0}},
            {JMP_NE, {0x01, 0x00}},
            {MOV_REG_REG, {Register::R0, Register::R1}},
            {RETURN, {0x00, 0x00}},
        },
        6,
    };

    // Disassemble
    std::cout << disassemble(program) << std::endl;

    // Run
    auto ret = Interpreter.run(program);
    std::cout << "R0: " << hexstr16(ret) << std::endl;
}
