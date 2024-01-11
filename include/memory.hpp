#pragma once

#include <cstdint>
#include <cstring>
#include <iostream>

#include "utils/fmt.hpp"
#include "utils/hexutils.hpp"

#include "instructions.hpp"

namespace memory
{
    struct Memory
    {
        uint8_t *memory;
        uint32_t size; // in bytes

        Memory(uint32_t size) : size(size)
        {
            this->memory = new uint8_t[size];
            std::memset(this->memory, 0, size);
        }
        ~Memory() { delete[] this->memory; }

        inline uint8_t *read(instructions::Addr address, size_t size)
        {
            uint8_t *data = new uint8_t[size];
            for (size_t i = 0; i < size; i++)
            {
                data[i] = this->memory[address + i];
            }
            return data;
        }

        inline uint8_t read8(instructions::Addr address) { return this->memory[address]; }
        inline uint16_t read16(instructions::Addr address) { return (this->memory[address] << 8) | this->memory[address + 1]; }
        inline uint32_t read32(instructions::Addr address) { return (this->memory[address] << 24) | (this->memory[address + 1] << 16) | (this->memory[address + 2] << 8) | this->memory[address + 3]; }

        inline size_t write(instructions::Addr address, uint8_t *data, size_t size)
        {
            for (size_t i = 0; i < size; i++)
                this->memory[address + i] = data[i];
            return size;
        }

        inline size_t write8(instructions::Addr address, uint8_t value)
        {
            this->memory[address] = value;
            return 1;
        }

        inline size_t write16(instructions::Addr address, uint16_t value)
        {
            this->memory[address] = (value >> 8) & 0xFF;
            this->memory[address + 1] = value & 0xFF;
            return 2;
        }

        inline size_t write32(instructions::Addr address, uint32_t value)
        {
            this->memory[address] = (value >> 24) & 0xFF;
            this->memory[address + 1] = (value >> 16) & 0xFF;
            this->memory[address + 2] = (value >> 8) & 0xFF;
            this->memory[address + 3] = value & 0xFF;
            return 4;
        }

        inline std::string to_string(uint32_t rows, uint32_t columns)
        {
            using namespace utils;
            std::string str = "";
            bool empty = false;
            bool prev_empty = false;
            for (size_t i = 0; i < rows; i++)
            {
                // check if row is empty
                empty = true;
                for (size_t j = 0; j < columns; j++)
                {
                    auto address = i * columns + j;
                    auto value = this->memory[address];
                    if (value != 0)
                    {
                        empty = false;
                        break;
                    }
                }
                if (empty && !prev_empty)
                {
                    str += colorize("| ", FG_BLUE, BOLD) + colorize("...\n", FG_BLACK, BOLD);
                    prev_empty = true;
                    continue;
                }
                else if (empty && prev_empty)
                    continue;
                else
                    prev_empty = false;
                str += colorize("| " + utils::hexstr32(i * columns), FG_BLUE, BOLD) + colorize(" | ", BOLD);
                for (size_t j = 0; j < columns; j++)
                {
                    if (j == 4 || j == 12)
                        str += " ";
                    if (j == 8)
                        str += colorize("| ", BOLD);
                    auto address = i * columns + j;

                    if (address >= this->size)
                        break;

                    auto value = this->memory[address];
                    std::stringstream s;
                    s << std::hex << std::setw(2) << std::setfill('0') << (int)value;
                    if (value == 0)
                    {
                        str += colorize(s.str(), FG_BLACK, BOLD) + " ";
                    }
                    else
                    {
                        str += colorize(s.str(), FG_BWHITE, BOLD) + " ";
                    }
                }
                str += colorize("| ", BOLD);
                for (size_t j = 0; j < columns; j++)
                {
                    auto address = i * columns + j;
                    auto value = this->memory[address];
                    if (value == 0)
                    {
                        str += colorize(". ", FG_BLACK, BOLD);
                    }
                    else
                    {
                        str += colorize(std::string(1, value), FG_BWHITE, BOLD);
                    }
                }
                str += colorize("|", BOLD);
                str += "\n";
            }
            return str;
        }
    };

    inline void inspect(Memory *memory)
    {
        using namespace utils;
        auto columns = 16;
        auto rows = memory->size / columns;

        std::cout << colorize("MEMORY", FG_BLUE, BOLD) << std::endl;
        std::cout << colorize("|", FG_BLUE, BOLD) << colorize(" Size: ", FG_GREEN, BOLD)
                  << colorize(std::to_string(memory->size) + " bytes", FG_BWHITE, BOLD) << std::endl;
        std::cout << colorize("|", FG_BLUE, BOLD) << colorize(" instructions::Addressable Range: ", FG_GREEN, BOLD)
                  << colorize("0x0000", FG_BWHITE, BOLD) << colorize(" - ", FG_BLUE, BOLD)
                  << colorize(utils::hexstr32(memory->size), FG_BWHITE, BOLD) << std::endl;
        std::cout << memory->to_string(rows, columns);
        std::cout << colorize("END MEMORY", FG_BLUE, BOLD) << std::endl;
    }
}
