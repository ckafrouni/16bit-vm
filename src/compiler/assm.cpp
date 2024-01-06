#include <string>
#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>

#include "compiler/assm.hpp"

#include "fmt.hpp"
#include "hexutils.hpp"

#include "instructions.hpp"
#include "addr.hpp"
#include "registers.hpp"
#include "memory.hpp"

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

// std::string stripWhitespace(const std::string &input)
// {
//     std::string output = input;
//     output.erase(std::remove_if(output.begin(), output.end(), ::isspace), output.end());
//     return output;
// }

Memory *compile(std::string source)
{
    auto program = new std::vector<uint8_t>();
    auto ip = 0x00;
    auto label_addresses = std::map<std::string, Addr>();
    auto unresolved_labels = std::map<std::string, Addr>();

    auto lines = split(source, '\n');
    for (auto line : lines)
    {
        // Strip whitespace
        if (line.empty())
            continue;

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
        if (instruction == "mov")
        {
            std::cout << fmt::colorize("Compiling MOV", fmt::Colors::FG_GREEN) << std::endl;
            if (tokens[1][0] == '$')
            {
                std::cout << fmt::colorize("Compiling MOV_LIT_REG", fmt::Colors::FG_GREEN) << std::endl;

                auto value = std::stoi(tokens[1].substr(1), nullptr, 0);
                auto reg = to_register(tokens[2].substr(1));

                program->push_back(MOV_LIT_REG);
                program->push_back(value >> 24);
                program->push_back(value >> 16);
                program->push_back(value >> 8);
                program->push_back(value);
                program->push_back(reg);
                ip += 6;
            }
            else
            {
                std::cout << fmt::colorize("Compiling MOV_REG_REG", fmt::Colors::FG_GREEN) << std::endl;

                auto reg1 = to_register(tokens[1]);
                auto reg2 = to_register(tokens[2]);

                program->push_back(MOV_REG_REG);
                program->push_back(reg1);
                program->push_back(reg2);
                ip += 3;
            }
            // TODO
        }

        /** PUSH instructions */
        else if (instruction == "push")
        {
            std::cout << fmt::colorize("Compiling PUSH", fmt::Colors::FG_GREEN) << std::endl;
            if (tokens[1][0] == '$')
            {
                std::cout << fmt::colorize("Compiling PUSH_LIT", fmt::Colors::FG_GREEN) << std::endl;

                auto value = std::stoi(tokens[1].substr(1), nullptr, 0);

                program->push_back(PUSH_LIT);
                program->push_back(value >> 24);
                program->push_back(value >> 16);
                program->push_back(value >> 8);
                program->push_back(value);
                ip += 5;
            }
            else
            {
                std::cout << fmt::colorize("Compiling PUSH_REG", fmt::Colors::FG_GREEN) << std::endl;

                auto reg = to_register(tokens[1].substr(1));

                program->push_back(PUSH_REG);
                program->push_back(reg);
                ip += 2;
            }
            // TODO
        }

        /** POP instructions */
        else if (instruction == "pop")
        {
            std::cout << fmt::colorize("Compiling POP", fmt::Colors::FG_GREEN) << std::endl;
            auto reg = to_register(tokens[1].substr(1));

            program->push_back(POP_REG);
            program->push_back(reg);
            ip += 2;
        }

        /** STORE instructions */
        // TODO

        /** LOAD instructions */
        // TODO

        /** ADD instructions */
        else if (instruction == "add")
        {
            std::cout << fmt::colorize("Compiling ADD", fmt::Colors::FG_GREEN) << std::endl;
            if (tokens[1][0] == '$')
            {
                std::cout << fmt::colorize("Compiling ADD_LIT_REG", fmt::Colors::FG_GREEN) << std::endl;

                auto value = std::stoi(tokens[1].substr(1), nullptr, 0);
                auto reg = to_register(tokens[2].substr(1));

                program->push_back(ADD_LIT_REG);
                program->push_back(value >> 24);
                program->push_back(value >> 16);
                program->push_back(value >> 8);
                program->push_back(value);
                program->push_back(reg);
                ip += 6;
            }
            else
            {
                std::cout << fmt::colorize("Compiling ADD_REG_REG", fmt::Colors::FG_GREEN) << std::endl;

                auto reg1 = to_register(tokens[1].substr(1));
                auto reg2 = to_register(tokens[2].substr(1));

                program->push_back(ADD_REG_REG);
                program->push_back(reg1);
                program->push_back(reg2);
                ip += 3;
            }
            // TODO
        }

        /** SUB instructions */
        // TODO

        /** INC instructions */
        else if (instruction == "inc")
        {
            std::cout << fmt::colorize("Compiling INC", fmt::Colors::FG_GREEN) << std::endl;
            // inc r1
            program->push_back(INC_REG);
            program->push_back(to_register(tokens[1].substr(1)));
            ip += 2;
            // TODO
        }

        /** DEC instructions */
        // TODO

        /** JMP instructions */
        else if (instruction == "jne")
        {
            std::cout << fmt::colorize("Compiling JMP_NE", fmt::Colors::FG_GREEN) << std::endl;
            // jmp_ne r1 loop (loop is a label -> 32bit address)

            auto label = tokens[2];
            auto label_address = label_addresses[label];

            program->push_back(JMP_NE);
            program->push_back(to_register(tokens[1].substr(1)));
            program->push_back(label_address >> 24);
            program->push_back(label_address >> 16);
            program->push_back(label_address >> 8);
            program->push_back(label_address);

            ip += 6;
            // TODO
        }

        /** CALL instructions */
        else if (instruction == "call")
        {
            std::cout << fmt::colorize("Compiling CALL", fmt::Colors::FG_GREEN) << std::endl;
            // call func (func is a label -> 32bit address)

            auto label = tokens[1];
            auto label_address = label_addresses[label];
            if (label_addresses.find(label) == label_addresses.end())
            {
                std::cout << fmt::colorize("Unresolved label: ", fmt::FG_RED, fmt::BOLD) << label << std::endl;
                unresolved_labels[label] = ip;
            }
            std::cout << fmt::colorize("label_address: ", fmt::FG_YELLOW, fmt::BOLD) << hexstr32(label_address) << std::endl;

            program->push_back(CALL_LIT);
            program->push_back(label_address >> 24);
            program->push_back(label_address >> 16);
            program->push_back(label_address >> 8);
            program->push_back(label_address);

            ip += 5;
            // TODO
        }

        /** RETURN instructions */
        else if (instruction == "ret")
        {
            std::cout << fmt::colorize("Compiling RETURN", fmt::Colors::FG_GREEN) << std::endl;
            program->push_back(RETURN);
            ip += 1;
        }

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
            // print file name and line number
            std::cout << fmt::colorize("Error on line: ", fmt::FG_RED, fmt::BOLD) << line << std::endl;
            // print file name and line number of this cpp file
            std::string f = __FILE__;
            auto l = std::to_string(__LINE__);
            std::cout << fmt::colorize(f + ":" + l, fmt::FG_RED, fmt::BOLD) << std::endl;
            exit(1);
        }

        // for (auto byte : *program)
        //      std::cout << fmt::colorize("byte: ", fmt::FG_YELLOW, fmt::BOLD) << hexstr16(byte) << std::endl;
    }

    // Resolve labels
    for (auto const &[label, address] : unresolved_labels)
    {
        std::cout << fmt::colorize("Resolving label: ", fmt::FG_YELLOW, fmt::BOLD) << label << std::endl;
        std::cout << fmt::colorize("Address: ", fmt::FG_YELLOW, fmt::BOLD) << hexstr32(address) << std::endl;
        auto label_address = label_addresses[label];
        std::cout << fmt::colorize("Label address: ", fmt::FG_YELLOW, fmt::BOLD) << hexstr32(label_address) << std::endl;
        program->at(address + 1) = label_address >> 24;
        program->at(address + 2) = label_address >> 16;
        program->at(address + 3) = label_address >> 8;
        program->at(address + 4) = label_address;
    }

    auto memory = new Memory(program->size());
    memory->write(0x00, program->data(), program->size());

    return memory;
}
