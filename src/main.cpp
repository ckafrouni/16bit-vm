#include <iostream>
#include <cstring>

#include "fmt.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "interpreter.hpp"
#include "instructions.hpp"
#include "addr.hpp"

int main()
{
    // Initialize
    uint32_t memory_size = 0xbeef;
    auto memory = Memory(memory_size);
    // auto s_addr = 0x00;
    auto s = "Hello, World!";
    // auto s2_addr = 0xbe00;
    auto s2 = "I'm Chris!";
    memory.write(0x00, (uint8_t *)s, strlen(s));
    memory.write(0xbe00, (uint8_t *)s2, strlen(s2));

    auto rf = RegisterFile{
        .registers = {
            0xdead, // R0
            0xbeef, // R1
            0xaaaa, // R2
            0xbbbb, // R3
            0x0000, // ACC
            0x0000, // IP
            0x0000, // SP
            0x0000, // FP
        },
    };

    Interpreter interpreter = {
        .memory = memory,
        .registers = rf,
        .modified_register = IP,
        .running = false,
    };

    auto program = Memory(0x1000);
    Addr main_addr = 0x00;
    Addr addr = main_addr;

    // MOV_LIT_REG 0x05 R1
    addr += program.write8(addr, (uint8_t)OpCode::MOV_LIT_REG);
    addr += program.write32(addr, 0x01);
    addr += program.write8(addr, (uint8_t)Register::R1);

    auto loop = addr;
    // INC_REG R1
    addr += program.write8(addr, (uint8_t)OpCode::INC_REG);
    addr += program.write8(addr, (uint8_t)Register::R1);

    // MOV_LIT_REG 0x05 ACC
    addr += program.write8(addr, (uint8_t)OpCode::MOV_LIT_REG);
    addr += program.write32(addr, 0x05);
    addr += program.write8(addr, (uint8_t)Register::ACC);

    // JMP_NE R1 0x00
    addr += program.write8(addr, (uint8_t)OpCode::JMP_NE);
    addr += program.write8(addr, (uint8_t)Register::R1);
    addr += program.write32(addr, loop);

    // // STORE_LIT_MEM 0xffffffff "Hey" // H: 0x48, e: 0x65, y: 0x79
    // addr += program.write8(addr, (uint8_t)OpCode::STORE_LIT_MEM);
    // addr += program.write32(addr, 0x486579);
    // addr += program.write32(addr, 0x1234);

    // // LOAD_MEM_REG 0x1234 R0
    // addr += program.write8(addr, (uint8_t)OpCode::LOAD_MEM_REG);
    // addr += program.write32(addr, 0x1234);
    // addr += program.write8(addr, (uint8_t)Register::R0);

    // RETURN
    addr += program.write8(addr, (uint8_t)OpCode::HALT); // RETURN

    // Inspect before running
    std::cout << fmt::colorize("## Inspection before running:", fmt::FG_RED, fmt::BOLD) << std::endl;
    std::cout << fmt::colorize("# Memory:", fmt::FG_WHITE, fmt::BOLD) << std::endl;
    interpreter.memory.inspect();
    std::cout << std::endl;
    std::cout << fmt::colorize("# Registers:", fmt::FG_WHITE, fmt::BOLD) << std::endl;
    interpreter.registers.inspect();
    std::cout << std::endl;
    std::cout << fmt::colorize("# Program:", fmt::FG_WHITE, fmt::BOLD) << std::endl;
    program.inspect();
    std::cout << std::endl;

    // Run
    auto ret = interpreter.run(&program, main_addr);

    // Inspect
    std::cout << std::endl;
    interpreter.memory.inspect();
    interpreter.registers.inspect();
}
