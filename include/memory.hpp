#pragma once

#include <cstdint>

#include "hexutils.hpp"
#include "fmt.hpp"

struct Memory
{
    uint8_t *memory;
    uint32_t size; // in bytes

    Memory(uint32_t size);
    ~Memory();

    uint8_t *read(uint32_t address, size_t size);
    uint8_t read8(uint32_t address);
    uint16_t read16(uint32_t address);
    uint32_t read32(uint32_t address);

    size_t write(uint32_t address, uint8_t *data, size_t size);
    size_t write8(uint32_t address, uint8_t value);
    size_t write16(uint32_t address, uint16_t value);
    size_t write32(uint32_t address, uint32_t value);

    std::string to_string(uint32_t rows, uint32_t columns);
    void inspect();
};
