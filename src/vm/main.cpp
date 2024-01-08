#include <iostream>
#include <fstream>
#include <cstring>

#include "utils/fmt.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "interpreter.hpp"
#include "instructions.hpp"

#include "assembly/compiler.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << utils::colorize("Usage: ", utils::FG_RED, utils::BOLD) << argv[0] << " <file>" << std::endl;
        return 1;
    }

    // Read file
    std::ifstream file(argv[1]);
    std::string source((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());

    // Compile
    auto cc = compiler::Compiler();
    cc.assemble(source);
    auto program = cc.get_program();

    std::cout << utils::colorize("## Compilation finished", utils::FG_RED, utils::BOLD) << std::endl;
    std::cout << utils::colorize("# Size of program: ", utils::FG_WHITE, utils::BOLD) << program->size << std::endl;
    std::cout << utils::colorize("# Program:", utils::FG_WHITE, utils::BOLD) << std::endl;
    memory::inspect(program);
    std::cout << std::endl;

    // Initialize
    static const uint32_t TOTAL_MEM_SIZE = 0x0000fff0;

    static const uint32_t START_ADDR = 0x00000000;
    static const uint32_t STACK_ADDR = TOTAL_MEM_SIZE;

    auto mem = memory::Memory(TOTAL_MEM_SIZE);
    // auto s_addr = 0x00;
    // auto s = "Hello, World!";
    // auto s2_addr = 0xbe00;
    // auto s2 = "I'm Chris!";
    // mem.write(0x00, (uint8_t *)s, strlen(s));
    // mem.write(0xbe00, (uint8_t *)s2, strlen(s2));

    auto rf = registers::RegisterFile();
    rf.set(registers::IP, START_ADDR);
    rf.set(registers::SP, STACK_ADDR);
    rf.set(registers::FP, rf.get(registers::SP));

    auto interpreter = vm::Interpreter(&mem, &rf);

    // interpreter.push(0xffffffff);
    // interpreter.push(0xeeeeeeee);
    // auto val = interpreter.pop();
    // std::cout << "Popped: " << std::hex << val << std::endl;
    // interpreter.push(0xddddd);

    // Inspect before running
    std::cout << utils::colorize("## Inspection before running:", utils::FG_RED, utils::BOLD) << std::endl;
    std::cout << utils::colorize("# Memory:", utils::FG_WHITE, utils::BOLD) << std::endl;
    memory::inspect(&mem);
    std::cout << std::endl;
    std::cout << utils::colorize("# Registers:", utils::FG_WHITE, utils::BOLD) << std::endl;
    registers::inspect(&rf);
    std::cout << std::endl;
    std::cout << utils::colorize("# Program:", utils::FG_WHITE, utils::BOLD) << std::endl;
    memory::inspect(program);
    std::cout << std::endl;

    // Run
    if (interpreter.load(program))
    {
        auto ret = interpreter.run(0x00, vm::DEBUG);
        (void)ret;
    }
    else
        std::cerr << utils::colorize("Failed to load program", utils::FG_RED, utils::BOLD) << std::endl;

    // Inspect
    std::cout << std::endl;
    std::cout << utils::colorize("## Inspection after running:", utils::FG_RED, utils::BOLD) << std::endl;
    std::cout << utils::colorize("# Memory:", utils::FG_WHITE, utils::BOLD) << std::endl;
    memory::inspect(&mem);
    std::cout << std::endl;
    std::cout << utils::colorize("# Registers:", utils::FG_WHITE, utils::BOLD) << std::endl;
    registers::inspect(&rf);
    std::cout << std::endl;
}
