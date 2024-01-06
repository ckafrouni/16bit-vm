#include <string>
#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>

#include "compiler/assm.hpp"

#include "utils/fmt.hpp"
#include "utils/hexutils.hpp"

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
    using namespace Instructions;

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

        std::cout << utils::colorize("line: ", utils::FG_YELLOW, utils::BOLD) << line << std::endl;
        auto tokens = split(line, ' ');
        auto instruction = tokens[0];
        std::cout << utils::colorize("compiling: ", utils::FG_YELLOW, utils::BOLD) << instruction << std::endl;
        std::cout << utils::colorize("tokens: ", utils::FG_YELLOW, utils::BOLD) << tokens.size() << std::endl;

        for (auto token : tokens)
        {
            std::cout << utils::colorize("token: ", utils::FG_YELLOW, utils::BOLD) << token << std::endl;
        }
        std::cout << utils::colorize("ip: ", utils::FG_YELLOW, utils::BOLD) << ip << std::endl;

        /** MOV instructions */
        if (instruction == "mov")
        {
            std::cout << utils::colorize("Compiling MOV", utils::Colors::FG_GREEN) << std::endl;
            if (tokens[1][0] == '$')
            {
                std::cout << utils::colorize("Compiling MOV_LIT_REG", utils::Colors::FG_GREEN) << std::endl;

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
                std::cout << utils::colorize("Compiling MOV_REG_REG", utils::Colors::FG_GREEN) << std::endl;

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
            std::cout << utils::colorize("Compiling PUSH", utils::Colors::FG_GREEN) << std::endl;
            if (tokens[1][0] == '$')
            {
                std::cout << utils::colorize("Compiling PUSH_LIT", utils::Colors::FG_GREEN) << std::endl;

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
                std::cout << utils::colorize("Compiling PUSH_REG", utils::Colors::FG_GREEN) << std::endl;

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
            std::cout << utils::colorize("Compiling POP", utils::Colors::FG_GREEN) << std::endl;
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
            std::cout << utils::colorize("Compiling ADD", utils::Colors::FG_GREEN) << std::endl;
            if (tokens[1][0] == '$')
            {
                std::cout << utils::colorize("Compiling ADD_LIT_REG", utils::Colors::FG_GREEN) << std::endl;

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
                std::cout << utils::colorize("Compiling ADD_REG_REG", utils::Colors::FG_GREEN) << std::endl;

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
            std::cout << utils::colorize("Compiling INC", utils::Colors::FG_GREEN) << std::endl;
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
            std::cout << utils::colorize("Compiling JMP_NE", utils::Colors::FG_GREEN) << std::endl;
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
            std::cout << utils::colorize("Compiling CALL", utils::Colors::FG_GREEN) << std::endl;
            // call func (func is a label -> 32bit address)

            auto label = tokens[1];
            auto label_address = label_addresses[label];
            if (label_addresses.find(label) == label_addresses.end())
            {
                std::cout << utils::colorize("Unresolved label: ", utils::FG_RED, utils::BOLD) << label << std::endl;
                unresolved_labels[label] = ip;
            }
            std::cout << utils::colorize("label_address: ", utils::FG_YELLOW, utils::BOLD) << utils::hexstr32(label_address) << std::endl;

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
            std::cout << utils::colorize("Compiling RETURN", utils::Colors::FG_GREEN) << std::endl;
            program->push_back(RETURN);
            ip += 1;
        }

        /** HALT instructions */
        else if (instruction == "halt")
        {
            std::cout << utils::colorize("Compiling HALT", utils::Colors::FG_GREEN) << std::endl;
            program->push_back(HALT);
            ip += 1;
        }

        /** LABEL instructions ("label_string:") */
        else if (instruction.back() == ':')
        {
            std::cout << utils::colorize("Compiling LABEL", utils::Colors::FG_GREEN) << std::endl;
            auto label = instruction.substr(0, instruction.length() - 1);
            label_addresses[label] = ip;
        }

        else
        {
            std::cout << utils::colorize("Unknown instruction: ", utils::FG_RED, utils::BOLD) << instruction << std::endl;
            // print file name and line number
            std::cout << utils::colorize("Error on line: ", utils::FG_RED, utils::BOLD) << line << std::endl;
            // print file name and line number of this cpp file
            std::string f = __FILE__;
            auto l = std::to_string(__LINE__);
            std::cout << utils::colorize(f + ":" + l, utils::FG_RED, utils::BOLD) << std::endl;
            exit(1);
        }

        // for (auto byte : *program)
        //      std::cout << utils::colorize("byte: ", utils::FG_YELLOW, utils::BOLD) << utils::hexstr16(byte) << std::endl;
    }

    // Resolve labels
    for (auto const &[label, address] : unresolved_labels)
    {
        std::cout << utils::colorize("Resolving label: ", utils::FG_YELLOW, utils::BOLD) << label << std::endl;
        std::cout << utils::colorize("Address: ", utils::FG_YELLOW, utils::BOLD) << utils::hexstr32(address) << std::endl;
        auto label_address = label_addresses[label];
        std::cout << utils::colorize("Label address: ", utils::FG_YELLOW, utils::BOLD) << utils::hexstr32(label_address) << std::endl;
        program->at(address + 1) = label_address >> 24;
        program->at(address + 2) = label_address >> 16;
        program->at(address + 3) = label_address >> 8;
        program->at(address + 4) = label_address;
    }

    auto memory = new Memory(program->size());
    memory->write(0x00, program->data(), program->size());

    return memory;
}
