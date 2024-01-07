#pragma once

#include <cstdint>
#include <string>

#include <iostream>
#include <string>
#include <algorithm>

#include "utils/hexutils.hpp"
#include "utils/fmt.hpp"

namespace registers
{
    // The total number of registers
    static const uint8_t REGISTER_COUNT = 8;

    enum RegisterEnum
    {
        ACC = 0x00,

        // General purpose registers
        R1,
        R2,
        R3,
        R4,

        // Special registers
        IP,
        SP,
        FP,
    };

    inline char *to_mnemonic(RegisterEnum reg)
    {
        switch (reg)
        {
        case RegisterEnum::ACC:
            return "ACC";
        case RegisterEnum::R1:
            return "R1";
        case RegisterEnum::R2:
            return "R2";
        case RegisterEnum::R3:
            return "R3";
        case RegisterEnum::R4:
            return "R4";
        case RegisterEnum::IP:
            return "IP";
        case RegisterEnum::SP:
            return "SP";
        case RegisterEnum::FP:
            return "FP";
        }
        throw std::runtime_error("Invalid register");
    }

    inline RegisterEnum to_register_enum(std::string reg)
    {
        std::transform(reg.begin(), reg.end(), reg.begin(), ::toupper);

        if (reg == "R1")
            return RegisterEnum::R1;
        else if (reg == "R2")
            return RegisterEnum::R2;
        else if (reg == "R3")
            return RegisterEnum::R3;
        else if (reg == "R4")
            return RegisterEnum::R4;
        else if (reg == "ACC")
            return RegisterEnum::ACC;
        else if (reg == "IP")
            return RegisterEnum::IP;
        else if (reg == "SP")
            return RegisterEnum::SP;
        else if (reg == "FP")
            return RegisterEnum::FP;

        throw std::runtime_error("Unknown register");
    }

    struct Register
    {
        RegisterEnum reg;
        const char *mnemonic;
        uint32_t value;

        Register(RegisterEnum reg, uint32_t value = 0)
            : reg(reg), value(value), mnemonic(to_mnemonic(reg)){};
    };

    class RegisterFile
    {
    private:
        Register registers[REGISTER_COUNT] = {
            Register{ACC},
            Register{R1}, Register{R2}, Register{R3}, Register{R4},
            Register{IP}, Register{SP}, Register{FP}};
        bool modified_registers[REGISTER_COUNT];

    public:
        inline void set(RegisterEnum reg, uint32_t value)
        {
            registers[reg].value = value;
            modified_registers[reg] = reg;
        }

        inline uint32_t get(RegisterEnum reg)
        {
            return registers[reg].value;
        }

        inline void inc(RegisterEnum reg, uint32_t value)
        {
            registers[reg].value += value;
            modified_registers[reg] = reg;
        }

        inline void dec(RegisterEnum reg, uint32_t value)
        {
            registers[reg].value -= value;
            modified_registers[reg] = reg;
        }

        inline void modify(RegisterEnum reg)
        {
            modified_registers[reg] = true;
        }

        inline bool is_modified(RegisterEnum reg)
        {
            return modified_registers[reg];
        }

        inline void reset_modified()
        {
            for (int i = 0; i < REGISTER_COUNT; i++)
                modified_registers[i] = RegisterEnum::ACC;
        }

        inline std::string to_string()
        {
            using namespace utils;
            std::string str = colorize("REGISTER FILE", FG_BCYAN, BOLD) += "\n";
            for (size_t i = IP; i <= FP; i++)
            {
                auto reg = (RegisterEnum)i;
                auto reg_str = colorize(to_mnemonic(reg), FG_GREEN, BOLD);
                str += colorize("|  ", FG_CYAN, BOLD) + reg_str + "  " + utils::hexstr32(get(reg));
                if (is_modified(reg))
                    str += colorize(" *", FG_YELLOW, BOLD);
                str += "\n";
            }

            auto reg_str = colorize(to_mnemonic(ACC), FG_BRED, BOLD);
            str += colorize("| ", FG_CYAN, BOLD) + reg_str + "  " + utils::hexstr32(get(ACC));
            if (is_modified(ACC))
                str += colorize(" *", FG_YELLOW, BOLD);
            str += "\n";

            for (size_t i = R1; i <= R4; i++)
            {
                auto reg = (RegisterEnum)i;
                auto reg_str = colorize(to_mnemonic(reg), FG_BPURPLE, BOLD);
                str += colorize("|  ", FG_CYAN, BOLD) + reg_str + "  " + utils::hexstr32(get(reg));
                if (is_modified(reg))
                    str += colorize(" *", FG_YELLOW, BOLD);
                str += "\n";
            }
            str += colorize("END REGISTER FILE", FG_BCYAN, BOLD);
            return str;
        }
    };

    inline void inspect(RegisterFile *registers)
    {
        std::cout << registers->to_string() << std::endl;
    }
} // namespace registers
