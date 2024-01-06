#include <iostream>
#include <fstream>
#include <cstring>

#include "fmt.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "interpreter.hpp"
#include "instructions.hpp"
#include "addr.hpp"

#include "compiler/assm.hpp"

int main(int argc, char **argv)
{
    // Read file
    std::ifstream file(argv[1]);
    std::string source((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());

    // Compile
    auto program = compile(source);

    // auto program = compile("mov $0x00 %r1\n"
    //                        "loop:\n"
    //                        // "push %r1\n"
    //                        // "call 'std::putchar'\n"
    //                        "inc %r1\n"
    //                        "mov $0x05 %acc\n"
    //                        "jne %r1 loop\n"
    //                        "halt\n");

    std::cout << fmt::colorize("## Compilation finished", fmt::FG_RED, fmt::BOLD) << std::endl;
    std::cout << fmt::colorize("# Size of program: ", fmt::FG_WHITE, fmt::BOLD) << program->size << std::endl;
    std::cout << fmt::colorize("# Program:", fmt::FG_WHITE, fmt::BOLD) << std::endl;
    program->inspect();
    std::cout << std::endl;

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

    // Inspect before running
    std::cout << fmt::colorize("## Inspection before running:", fmt::FG_RED, fmt::BOLD) << std::endl;
    std::cout << fmt::colorize("# Memory:", fmt::FG_WHITE, fmt::BOLD) << std::endl;
    interpreter.memory.inspect();
    std::cout << std::endl;
    std::cout << fmt::colorize("# Registers:", fmt::FG_WHITE, fmt::BOLD) << std::endl;
    interpreter.registers.inspect();
    std::cout << std::endl;
    std::cout << fmt::colorize("# Program:", fmt::FG_WHITE, fmt::BOLD) << std::endl;
    program->inspect();
    std::cout << std::endl;

    // Run
    auto ret = interpreter.run(program, 0x00, Mode::RELEASE);
    (void)ret;

    // Inspect
    std::cout << std::endl;
    interpreter.memory.inspect();
    interpreter.registers.inspect();
}
