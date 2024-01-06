#pragma once

#include <cstdint>

#include "hexutils.hpp"
#include "fmt.hpp"
#include "addr.hpp"

struct Memory
{
    uint8_t *memory;
    uint32_t size; // in bytes

    Memory(uint32_t size);
    ~Memory();

    uint8_t *read(Addr address, size_t size);
    uint8_t read8(Addr address);
    uint16_t read16(Addr address);
    uint32_t read32(Addr address);

    size_t write(Addr address, uint8_t *data, size_t size);
    size_t write8(Addr address, uint8_t value);
    size_t write16(Addr address, uint16_t value);
    size_t write32(Addr address, uint32_t value);

    std::string to_string(uint32_t rows, uint32_t columns);
    void inspect();
};
