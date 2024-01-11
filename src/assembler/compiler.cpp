#include <string>
#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>

#include "assembler/compiler.hpp"

#include "utils/fmt.hpp"
#include "utils/hexutils.hpp"
#include "utils/strmanip.hpp"

#include "instructions.hpp"

#include "registers.hpp"
#include "memory.hpp"

using namespace instructions;

void compiler::Compiler::assemble(std::string source)
{

    auto label_addresses = std::map<std::string, Addr>();
    auto unresolved_labels = std::map<std::string, std::vector<Addr>>();

    auto lines = utils::split(source, '\n');
    for (auto line : lines)
    {
        // Strip whitespace
        if (line.empty())
            continue;

        std::cout << std::endl;
        std::cout << utils::colorize("line: ", utils::FG_YELLOW, utils::BOLD) << line << std::endl;
        auto tokens = utils::split(line, ' ');
        std::cout << utils::colorize("number of tokens: ", utils::FG_YELLOW, utils::BOLD) << tokens.size() << std::endl;

        for (auto token : tokens)
            std::cout << utils::colorize("  token: ", utils::FG_YELLOW, utils::BOLD) << token << std::endl;

        auto instruction = tokens[0];

        /** MOV instructions */
        if (instruction == "mov")
        {
            std::cout << utils::colorize("Compiling MOV", utils::Colors::FG_GREEN) << std::endl;

            if (tokens[1][0] == '$')
            {
                std::cout << utils::colorize("Compiling MOV_LIT_REG", utils::Colors::FG_GREEN) << std::endl;

                auto value = std::stoi(tokens[1].substr(1), nullptr, 0);
                auto reg = registers::to_register_enum(tokens[2].substr(1));

                this->current_addr += encode_mov_lit_reg(this->current_addr, value, reg);
            }
            else
            {
                std::cout << utils::colorize("Compiling MOV_REG_REG", utils::Colors::FG_GREEN) << std::endl;

                auto reg1 = registers::to_register_enum(tokens[1]);
                auto reg2 = registers::to_register_enum(tokens[2]);

                this->current_addr += encode_mov_reg_reg(this->current_addr, reg1, reg2);
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

                this->program->write8(this->current_addr, PUSH_LIT);
                this->program->write32(this->current_addr + 1, value);
                this->current_addr += 5; // Opcode + Value
            }
            else
            {
                std::cout << utils::colorize("Compiling PUSH_REG", utils::Colors::FG_GREEN) << std::endl;

                auto reg = registers::to_register_enum(tokens[1].substr(1));

                this->program->write8(this->current_addr, PUSH_REG);
                this->program->write8(this->current_addr + 1, reg);
                this->current_addr += 2; // Opcode + Reg
            }
            // TODO
        }

        /** POP instructions */
        else if (instruction == "pop")
        {
            std::cout << utils::colorize("Compiling POP", utils::Colors::FG_GREEN) << std::endl;
            auto reg = registers::to_register_enum(tokens[1].substr(1));

            this->program->write8(this->current_addr, POP_REG);
            this->program->write8(this->current_addr + 1, reg);
            this->current_addr += 2; // Opcode + Reg
        }

        /** ADD instructions */
        else if (instruction == "add")
        {
            std::cout << utils::colorize("Compiling ADD", utils::Colors::FG_GREEN) << std::endl;
            if (tokens[1][0] == '$')
            {
                std::cout << utils::colorize("Compiling ADD_LIT_REG", utils::Colors::FG_GREEN) << std::endl;

                auto value = std::stoi(tokens[1].substr(1), nullptr, 0);
                auto reg = registers::to_register_enum(tokens[2].substr(1));

                this->program->write8(this->current_addr, ADD_LIT_REG);
                this->program->write32(this->current_addr + 1, value);
                this->program->write8(this->current_addr + 5, reg);
                this->current_addr += 6;
            }
            else
            {
                std::cout << utils::colorize("Compiling ADD_REG_REG_REG", utils::Colors::FG_GREEN) << std::endl;
                // add %<dst> %<src1> %<src2>

                auto reg1 = registers::to_register_enum(tokens[1].substr(1));
                auto reg2 = registers::to_register_enum(tokens[2].substr(1));
                auto reg3 = registers::to_register_enum(tokens[3].substr(1));

                this->program->write8(this->current_addr, ADD_REG_REG_REG);
                this->program->write8(this->current_addr + 1, reg1);
                this->program->write8(this->current_addr + 2, reg2);
                this->program->write8(this->current_addr + 3, reg3);
                this->current_addr += 4; // Opcode + Reg + Reg + Reg
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
            this->program->write8(this->current_addr, INC_REG);
            this->program->write8(this->current_addr + 1, registers::to_register_enum(tokens[1].substr(1)));
            this->current_addr += 2; // Opcode + Reg
            // TODO
        }

        /** DEC instructions */
        else if (instruction == "dec")
        {
            std::cout << utils::colorize("Compiling DEC", utils::Colors::FG_GREEN) << std::endl;
            // dec r1
            this->program->write8(this->current_addr, DEC_REG);
            this->program->write8(this->current_addr + 1, registers::to_register_enum(tokens[1].substr(1)));
            this->current_addr += 2; // Opcode + Reg
            // TODO
        }

        /** JMP instructions */
        else if (instruction == "jmp")
        {
            std::cout << utils::colorize("Compiling JMP", utils::Colors::FG_GREEN) << std::endl;
            // jmp <label>

            this->program->write8(this->current_addr, JMP);

            auto label = tokens[1];
            unresolved_labels[label].push_back(this->current_addr + 1);

            this->current_addr += 5; // Opcode + Addr
        }
        else if (instruction == "jne")
        {
            std::cout << utils::colorize("Compiling JMP_NE", utils::Colors::FG_GREEN) << std::endl;
            // jne %<reg> %<reg> <label>

            this->program->write8(this->current_addr, JMP_NE);
            this->program->write8(this->current_addr + 1, registers::to_register_enum(tokens[1].substr(1)));
            this->program->write8(this->current_addr + 2, registers::to_register_enum(tokens[2].substr(1)));

            auto label = tokens[3];
            unresolved_labels[label].push_back(this->current_addr + 3);

            this->current_addr += 7; // Opcode + Reg + Reg + Addr
        }
        else if (instruction == "je")
        {
            std::cout << utils::colorize("Compiling JMP_EQ", utils::Colors::FG_GREEN) << std::endl;
            // je %<reg> %<reg> <label>

            this->program->write8(this->current_addr, JMP_EQ);
            this->program->write8(this->current_addr + 1, registers::to_register_enum(tokens[1].substr(1)));
            this->program->write8(this->current_addr + 2, registers::to_register_enum(tokens[2].substr(1)));

            auto label = tokens[3];
            unresolved_labels[label].push_back(this->current_addr + 3);

            this->current_addr += 7; // Opcode + Reg + Reg + Addr
        }
        else if (instruction == "jge")
        {
            std::cout << utils::colorize("Compiling JMP_GE", utils::Colors::FG_GREEN) << std::endl;
            // jge %<reg> %<reg> <label>

            this->program->write8(this->current_addr, JMP_GE);
            this->program->write8(this->current_addr + 1, registers::to_register_enum(tokens[1].substr(1)));
            this->program->write8(this->current_addr + 2, registers::to_register_enum(tokens[2].substr(1)));

            auto label = tokens[3];
            unresolved_labels[label].push_back(this->current_addr + 3);

            this->current_addr += 7; // Opcode + Reg + Reg + Addr
        }
        else if (instruction == "jg")
        {
            std::cout << utils::colorize("Compiling JMP_G", utils::Colors::FG_GREEN) << std::endl;
            // jg %<reg> %<reg> <label>

            this->program->write8(this->current_addr, JMP_GT);
            this->program->write8(this->current_addr + 1, registers::to_register_enum(tokens[1].substr(1)));
            this->program->write8(this->current_addr + 2, registers::to_register_enum(tokens[2].substr(1)));

            auto label = tokens[3];
            unresolved_labels[label].push_back(this->current_addr + 3);

            this->current_addr += 7; // Opcode + Reg + Reg + Addr
        }
        else if (instruction == "jl")
        {
            std::cout << utils::colorize("Compiling JMP_LT", utils::Colors::FG_GREEN) << std::endl;
            // jl %<reg> %<reg> <label>

            this->program->write8(this->current_addr, JMP_LT);
            this->program->write8(this->current_addr + 1, registers::to_register_enum(tokens[1].substr(1)));
            this->program->write8(this->current_addr + 2, registers::to_register_enum(tokens[2].substr(1)));

            auto label = tokens[3];
            unresolved_labels[label].push_back(this->current_addr + 3);

            this->current_addr += 7; // Opcode + Reg + Reg + Addr
        }
        else if (instruction == "jle")
        {
            std::cout << utils::colorize("Compiling JMP_LE", utils::Colors::FG_GREEN) << std::endl;
            // jle %<reg> %<reg> <label>

            this->program->write8(this->current_addr, JMP_LE);
            this->program->write8(this->current_addr + 1, registers::to_register_enum(tokens[1].substr(1)));
            this->program->write8(this->current_addr + 2, registers::to_register_enum(tokens[2].substr(1)));

            auto label = tokens[3];
            unresolved_labels[label].push_back(this->current_addr + 3);

            this->current_addr += 7; // Opcode + Reg + Reg + Addr
        }

        /** CALL instructions */
        else if (instruction == "call")
        {
            std::cout << utils::colorize("Compiling CALL", utils::Colors::FG_GREEN) << std::endl;
            // call <func>

            this->program->write8(this->current_addr, CALL);

            auto label = tokens[1];
            unresolved_labels[label].push_back(this->current_addr + 1);

            this->current_addr += 5; // Opcode + Addr
        }

        /** RETURN instructions */
        else if (instruction == "ret")
        {
            std::cout << utils::colorize("Compiling RETURN", utils::Colors::FG_GREEN) << std::endl;
            this->program->write8(this->current_addr, RETURN);
            this->current_addr += 1;
        }

        /** HALT instructions */
        else if (instruction == "halt")
        {
            std::cout << utils::colorize("Compiling HALT", utils::Colors::FG_GREEN) << std::endl;
            this->program->write8(this->current_addr, HALT);
            this->current_addr += 1;
        }

        /** LABEL instructions ("label_string:") */
        else if (instruction.back() == ':')
        {
            std::cout << utils::colorize("Compiling LABEL", utils::Colors::FG_GREEN) << std::endl;
            auto label = instruction.substr(0, instruction.length() - 1);
            std::cout << utils::colorize("label: ", utils::FG_YELLOW, utils::BOLD) << label << std::endl;

            // Add label to map
            label_addresses[label] = this->current_addr;
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
        std::cout << utils::colorize("current_addr: ", utils::FG_YELLOW, utils::BOLD) << utils::hexstr32(this->current_addr) << std::endl;
    }

    for (auto const &[label, addresses] : unresolved_labels)
    {
        for (auto address : addresses)
        {
            this->program->write32(address, label_addresses[label]);
        }
    }
}
