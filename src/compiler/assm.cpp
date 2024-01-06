#include <string>
#include <iostream>
#include <cstdint>
#include <vector>

#include "compiler/assm.hpp"

#include "fmt.hpp"
#include "hexutils.hpp"

#include "instructions.hpp"
#include "addr.hpp"
#include "registers.hpp"
#include "memory.hpp"

#include <string>
#include <sstream>
#include <vector>
#include <map>

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

Memory *compile(std::string source)
{
    auto program = new std::vector<uint8_t>();
    auto ip = 0x00;
    auto label_addresses = std::map<std::string, Addr>();

    auto lines = split(source, '\n');
    for (auto line : lines)
    {
        std::cout << fmt::colorize("line: ", fmt::FG_YELLOW, fmt::BOLD) << line << std::endl;
        auto tokens = split(line, ' ');
        auto instruction = tokens[0];
        std::cout << fmt::colorize("compiling: ", fmt::FG_YELLOW, fmt::BOLD) << instruction << std::endl;
        std::cout << fmt::colorize("tokens: ", fmt::FG_YELLOW, fmt::BOLD) << tokens.size() << std::endl;

        for (auto token : tokens)
        {
            std::cout << fmt::colorize("token: ", fmt::FG_YELLOW, fmt::BOLD) << token << std::endl;
        }
        std::cout << fmt::colorize("ip: ", fmt::FG_YELLOW, fmt::BOLD) << ip << std::endl;

        /** MOV instructions */
        if (instruction == "mov_lit_reg")
        {
            std::cout << fmt::colorize("Compiling MOV_LIT_REG", fmt::Colors::FG_GREEN) << std::endl;

            auto value = std::stoi(tokens[1], nullptr, 0);
            auto reg = to_register(tokens[2]);

            program->push_back(MOV_LIT_REG);
            program->push_back(value >> 24);
            program->push_back(value >> 16);
            program->push_back(value >> 8);
            program->push_back(value);
            program->push_back(reg);
            ip += 6;
        }
        // TODO

        /** STORE instructions */
        // TODO

        /** LOAD instructions */
        // TODO

        /** ADD instructions */
        // TODO

        /** SUB instructions */
        // TODO

        /** INC instructions */
        else if (instruction == "inc_reg")
        {
            std::cout << fmt::colorize("Compiling INC_REG", fmt::Colors::FG_GREEN) << std::endl;
            program->push_back(INC_REG);
            program->push_back(to_register(tokens[1]));
            ip += 2;
        }
        // TODO

        /** DEC instructions */
        // TODO

        /** JMP instructions */
        else if (instruction == "jmp_ne")
        {
            std::cout << fmt::colorize("Compiling JMP_NE", fmt::Colors::FG_GREEN) << std::endl;
            // jmp_ne r1 loop (loop is a label -> 32bit address)

            auto label = tokens[2];
            auto label_address = label_addresses[label];

            program->push_back(JMP_NE);
            program->push_back(to_register(tokens[1])); // r1
            program->push_back(label_address >> 24);
            program->push_back(label_address >> 16);
            program->push_back(label_address >> 8);
            program->push_back(label_address);

            ip += 6;
        }
        // TODO

        /** CALL instructions */
        // TODO

        /** RETURN instructions */
        // TODO

        /** HALT instructions */
        else if (instruction == "halt")
        {
            std::cout << fmt::colorize("Compiling HALT", fmt::Colors::FG_GREEN) << std::endl;
            program->push_back(HALT);
            ip += 1;
        }

        /** LABEL instructions ("label_string:") */
        else if (instruction.back() == ':')
        {
            std::cout << fmt::colorize("Compiling LABEL", fmt::Colors::FG_GREEN) << std::endl;
            auto label = instruction.substr(0, instruction.length() - 1);
            label_addresses[label] = ip;
        }

        else
        {
            std::cout << fmt::colorize("Unknown instruction: ", fmt::FG_RED, fmt::BOLD) << instruction << std::endl;
            exit(1);
        }

        // for (auto byte : *program)
        //      std::cout << fmt::colorize("byte: ", fmt::FG_YELLOW, fmt::BOLD) << hexstr16(byte) << std::endl;
    }

    auto memory = new Memory(program->size());
    memory->write(0x00, program->data(), program->size());

    return memory;
}
