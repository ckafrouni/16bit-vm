#pragma once

#include <string>

#include "instructions.hpp"

std::string hexstr16(unsigned short n);
std::string hexstr32(unsigned int n);
std::string disassemble(Program program);

void Interpreter::inspect(Register changed_register)
{
    std::cout << fmt::colorize("Registers", fmt::Colors::FG_BPURPLE, fmt::BOLD) << std::endl;
    std::cout << "IP: " + hexstr16(ip) << std::endl;
    std::cout << "ZF: " + std::to_string(zf) << std::endl;
    std::cout << "CF: " + std::to_string(cf) << std::endl;
    std::cout << "--------" << std::endl;
    for (size_t i = 0; i < 4; i++)
    {
        if (i == changed_register)
            std::cout << fmt::colorize("R" + std::to_string(i) + ": " + hexstr16(registers[i]), fmt::Colors::FG_BGREEN, fmt::BOLD) << std::endl;
        else
            std::cout << "R" << i << ": " << hexstr16(registers[i]) << std::endl;
    }
}

void Interpreter::inspect()
{
    std::cout << fmt::colorize("Registers", fmt::Colors::FG_BPURPLE, fmt::BOLD) << std::endl;
    for (size_t i = 0; i < 4; i++)
        std::cout << "R" << i << ": " << hexstr16(registers[i]) << std::endl;
}

std::string hexstr16(unsigned short n)
{
    std::stringstream stream;
    auto prefix = fmt::colorize("0x", fmt::Colors::FG_BCYAN, fmt::BOLD);
    stream << prefix << std::hex << std::setw(2) << std::setfill('0') << n;
    return stream.str();
}

std::string hexstr32(unsigned int n)
{
    std::stringstream stream;
    stream << "0x" << std::hex << std::setw(4) << std::setfill('0') << n;
    return stream.str();
}
