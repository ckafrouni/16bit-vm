#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "assembler/compiler.hpp"
#include "utils/fmt.hpp"

/**
 * Compiler for the assembly language
 *
 * Usage: cc [OPTIONS] <file>
 *
 * Options:
 *      -h, --help      Print this help message
 *      -o <file>       Output file
 */

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << utils::colorize("Usage: ", utils::FG_RED, utils::BOLD) << argv[0] << " <file>" << std::endl;
        return 1;
    }

    // Parse arguments
    std::string input_file;
    std::string output_file = "out.bin";

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            std::cout << utils::colorize("Usage: ", utils::FG_RED, utils::BOLD) << argv[0] << " [OPTIONS] <file>" << std::endl;
            std::cout << utils::colorize("Options:", utils::FG_RED, utils::BOLD) << std::endl;
            std::cout << utils::colorize("    -h, --help      Print this help message", utils::FG_WHITE, utils::BOLD) << std::endl;
            std::cout << utils::colorize("    -o <file>       Output file", utils::FG_WHITE, utils::BOLD) << std::endl;
            return 0;
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            if (i + 1 < argc)
            {
                output_file = argv[i + 1];
                i++;
            }
            else
            {
                std::cerr << utils::colorize("Missing argument for option ", utils::FG_RED, utils::BOLD) << argv[i] << std::endl;
                return 1;
            }
        }
        else
        {
            input_file = argv[i];
        }
    }

    // Read file
    std::ifstream file(input_file);
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

    // Dump to file
    std::ofstream out(output_file, std::ios::out | std::ios::binary);
    out.write((char *)program->memory, program->size);
    out.close();

    const int permissions = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; // This sets the permissions to 755
    if (chmod(output_file.c_str(), permissions) != 0)
    {
        std::cerr << "Error setting executable permissions on: " << output_file << std::endl;
        // Handle error
    }

    return 0;
}
