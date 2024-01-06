#include <iostream>
#include <fstream>
#include <cstring>

#include "utils/fmt.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "interpreter.hpp"
#include "instructions.hpp"
#include "addr.hpp"

#include "compiler/assm.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << utils::colorize("Usage: ", utils::FG_RED, utils::BOLD) << argv[0] << " <file>" << std::endl;
    }

    // Read file
    std::ifstream file(argv[1]);
    std::string source((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());

    // Compile
    auto program = compile(source);

    std::cout << utils::colorize("## Compilation finished", utils::FG_RED, utils::BOLD) << std::endl;
    std::cout << utils::colorize("# Size of program: ", utils::FG_WHITE, utils::BOLD) << program->size << std::endl;
    std::cout << utils::colorize("# Program:", utils::FG_WHITE, utils::BOLD) << std::endl;
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
            0xffff, // SP
            0xffff, // FP
        },
    };

    Interpreter interpreter = {
        .memory = memory,
        .registers = rf,
        .modified_register = IP,
        .running = false,
    };

    // Inspect before running
    std::cout << utils::colorize("## Inspection before running:", utils::FG_RED, utils::BOLD) << std::endl;
    std::cout << utils::colorize("# Memory:", utils::FG_WHITE, utils::BOLD) << std::endl;
    interpreter.memory.inspect();
    std::cout << std::endl;
    std::cout << utils::colorize("# Registers:", utils::FG_WHITE, utils::BOLD) << std::endl;
    interpreter.registers.inspect();
    std::cout << std::endl;
    std::cout << utils::colorize("# Program:", utils::FG_WHITE, utils::BOLD) << std::endl;
    program->inspect();
    std::cout << std::endl;

    // Run
    auto ret = interpreter.run(program, 0x00, Mode::DEBUG);
    (void)ret;

    // Inspect
    std::cout << std::endl;
    interpreter.memory.inspect();
    interpreter.registers.inspect();
}
