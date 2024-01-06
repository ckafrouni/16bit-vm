#include <string>
#include <iomanip>

#include "fmt.hpp"

std::string hexstr8(uint8_t n)
{
    std::stringstream stream;
    stream << "0x" << std::hex << std::setw(2) << std::setfill('0') << n;
    return stream.str();
}

std::string hexstr16(uint16_t n)
{
    std::stringstream stream;
    stream << std::hex << std::setw(4) << std::setfill('0') << n;
    auto prefix = fmt::colorize("0x", fmt::Colors::FG_BLACK, fmt::BOLD);
    std::string value;
    if (n != 0)
        value = fmt::colorize(stream.str(), fmt::Colors::FG_WHITE, fmt::BOLD);
    else
        value = fmt::colorize(stream.str(), fmt::Colors::FG_BLACK, fmt::BOLD);
    return prefix + value;
}

std::string hexstr32(uint32_t n)
{
    std::stringstream stream;
    stream << "0x" << std::hex << std::setw(8) << std::setfill('0') << n;
    return stream.str();
}
