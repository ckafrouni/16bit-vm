#pragma once

#include <string>
#include <iomanip>

#include "utils/fmt.hpp"

namespace utils
{
    inline std::string hexstr8(uint8_t n)
    {
        std::stringstream stream;
        stream << std::setw(2) << std::setfill('0') << std::hex << n;
        auto prefix = colorize("0x", Colors::FG_BLACK, BOLD);
        std::string value;
        if (n != 0)
            value = colorize(stream.str(), Colors::FG_WHITE, BOLD);
        else
            value = colorize(stream.str(), Colors::FG_BLACK, BOLD);
        return prefix + value;
    };

    inline std::string hexstr16(uint16_t n)
    {
        std::stringstream stream;
        stream << std::hex << std::setw(4) << std::setfill('0') << n;
        auto prefix = colorize("0x", Colors::FG_BLACK, BOLD);
        std::string value;
        if (n != 0)
            value = colorize(stream.str(), Colors::FG_WHITE, BOLD);
        else
            value = colorize(stream.str(), Colors::FG_BLACK, BOLD);
        return prefix + value;
    };

    inline std::string hexstr32(uint32_t n)
    {
        std::stringstream stream;
        stream << std::hex << std::setw(8) << std::setfill('0') << n;
        auto prefix = colorize("0x", Colors::FG_BLACK, BOLD);
        std::string value;
        if (n != 0)
            value = colorize(stream.str(), Colors::FG_WHITE, BOLD);
        else
            value = colorize(stream.str(), Colors::FG_BLACK, BOLD);
        return prefix + value;
    };
}
