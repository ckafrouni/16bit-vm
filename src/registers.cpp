#include <iostream>
#include <string>
#include <algorithm>

#include "registers.hpp"
#include "fmt.hpp"
#include "hexutils.hpp"

uint32_t &RegisterFile::operator[](Register reg)
{
    return this->registers[reg];
}

std::string RegisterFile::to_string()
{
    using namespace fmt;
    std::string str = colorize("REGISTER FILE", FG_BCYAN, BOLD) += "\n";
    str += colorize("| ", FG_CYAN, BOLD) + colorize("IP ", FG_PURPLE, BOLD) + " " + hexstr32(this->registers[Register::IP]) + "\n";
    str += colorize("| ", FG_CYAN, BOLD) + colorize("SP ", FG_PURPLE, BOLD) + " " + colorize("0xTODO", FG_BLACK) + "\n";
    str += colorize("| ", FG_CYAN, BOLD) + colorize("FP ", FG_PURPLE, BOLD) + " " + colorize("0xTODO", FG_BLACK) + "\n";
    str += colorize("| ", FG_CYAN, BOLD) + colorize("ACC", FG_PURPLE, BOLD) + " " + hexstr32(this->registers[Register::ACC]) + "\n";
    for (size_t i = Register::R0; i <= Register::R3; i++)
    {
        auto reg = colorize("R" + std::to_string(i), FG_GREEN, BOLD);
        str += colorize("| ", FG_CYAN, BOLD) + reg + "  " + hexstr32(this->registers[i]) + "\n";
    }
    str += colorize("END REGISTER FILE", FG_BCYAN, BOLD);
    return str;
}

std::string RegisterFile::to_string(Register modified_register)
{
    using namespace fmt;
    std::string str = colorize("REGISTER FILE", FG_BCYAN, BOLD) += "\n";
    str += colorize("| ", FG_CYAN, BOLD) + colorize("IP ", FG_PURPLE, BOLD) + " " + hexstr32(this->registers[Register::IP]);
    if (modified_register == Register::IP)
        str += colorize(" *", FG_YELLOW, BOLD);
    str += "\n";

    str += colorize("| ", FG_CYAN, BOLD) + colorize("SP ", FG_PURPLE, BOLD) + " " + colorize("0xTODO", FG_BLACK) + "\n";
    str += colorize("| ", FG_CYAN, BOLD) + colorize("FP ", FG_PURPLE, BOLD) + " " + colorize("0xTODO", FG_BLACK) + "\n";
    str += colorize("| ", FG_CYAN, BOLD) + colorize("ACC", FG_PURPLE, BOLD) + " " + hexstr32(this->registers[Register::ACC]);
    if (modified_register == Register::ACC)
        str += colorize(" *", FG_YELLOW, BOLD);
    str += "\n";
    for (size_t i = Register::R0; i <= Register::R3; i++)
    {
        auto reg = colorize("R" + std::to_string(i), FG_GREEN, BOLD);
        str += colorize("| ", FG_CYAN, BOLD) + reg + "  " + hexstr32(this->registers[i]);
        if (modified_register == (Register)i)
            str += colorize(" *", FG_YELLOW, BOLD);
        str += "\n";
    }
    str += colorize("END REGISTER FILE", FG_BCYAN, BOLD);
    return str;
}

void RegisterFile::inspect()
{
    std::cout << this->to_string() << std::endl;
}

void RegisterFile::inspect(Register modified_register)
{
    std::cout << this->to_string(modified_register) << std::endl;
}

std::string to_string(Register reg)
{
    switch (reg)
    {
    case Register::R0:
        return "R0";
    case Register::R1:
        return "R1";
    case Register::R2:
        return "R2";
    case Register::R3:
        return "R3";
    case Register::IP:
        return "IP";
    default:
        return "UNKNOWN";
    }
}

Register to_register(std::string reg)
{
    std::transform(reg.begin(), reg.end(), reg.begin(), ::toupper);

    if (reg == "R0")
        return Register::R0;
    else if (reg == "R1")
        return Register::R1;
    else if (reg == "R2")
        return Register::R2;
    else if (reg == "R3")
        return Register::R3;
    else if (reg == "ACC")
        return Register::ACC;
    else if (reg == "IP")
        return Register::IP;
    else if (reg == "SP")
        return Register::SP;
    else if (reg == "FP")
        return Register::FP;

    std::cerr << "Unknown register: " << reg << std::endl;
    exit(1);
}
