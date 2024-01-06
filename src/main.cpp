#include <iostream>
#include <cstring>

#include "memory.hpp"
#include "registers.hpp"
#include "interpreter.hpp"
#include "instructions.hpp"

int main()
{
    // Initialize
    uint32_t memory_size = 0xbeef;
    auto memory = Memory(memory_size);
    auto s = "Hello, World!";
    auto s2 = "I'm Chris!";
    memory.write(0x00, (uint8_t *)s, strlen(s));
    memory.write(0xbedb, (uint8_t *)s2, strlen(s2));

    auto rf = RegisterFile{
        .registers = {
            0xdead, // R0
            0xbeef, // R1
            0xaaaa, // R2
            0xbbbb, // R3
            0x0000, // IP
        },
    };

    Interpreter interpreter = {
        .memory = memory,
        .registers = rf,
    };

    interpreter.memory.inspect();
    interpreter.registers.inspect();

    auto program = Memory(0x1000);
    auto main_addr = 0x00;
    program.write8(main_addr, (uint8_t)OpCode::MOV_LIT_REG); // MOV_LIT_REG 0x05 R1
    program.write32(main_addr + 1, 0x12121212);
    program.write8(main_addr + 5, (uint8_t)Register::R1);
    program.write8(main_addr + 6, (uint8_t)OpCode::RETURN); // RETURN

    program.inspect();

    // Run
    auto ret = interpreter.run(&program, main_addr);
    std::cout << "R0: " << hexstr16(ret) << std::endl;
}
