#pragma once

#include <string>

namespace fmt
{
    enum Colors
    {
        FG_BLACK = 30,
        FG_RED,
        FG_GREEN,
        FG_YELLOW,
        FG_BLUE,
        FG_PURPLE,
        FG_CYAN,
        FG_WHITE,
        FG_BBLACK = 90,
        FG_BRED,
        FG_BGREEN,
        FG_BYELLOW,
        FG_BBLUE,
        FG_BPURPLE,
        FG_BCYAN,
        FG_BWHITE,
        BG_BLACK = 40,
        BG_RED,
        BG_GREEN,
        BG_YELLOW,
        BG_BLUE,
        BG_PURPLE,
        BG_CYAN,
        BG_WHITE,
        BG_BBLACK = 100,
        BG_BRED,
        BG_BGREEN,
        BG_BYELLOW,
        BG_BBLUE,
        BG_BPURPLE,
        BG_BCYAN,
        BG_BWHITE
    };

    enum Styles
    {
        RESET = 0,
        BOLD = 1,
        UNDERLINE = 4,
    };

    template <typename... Args>
    std::string colorize(const std::string &str, Args... args)
    {
        std::string result = "\033[";
        int codes[] = {args...};
        for (size_t i = 0; i < sizeof(codes) / sizeof(int); i++)
        {
            result += std::to_string(codes[i]);
            if (i < sizeof(codes) / sizeof(int) - 1)
            {
                result += ";";
            }
        }
        result += "m" + str + "\033[0m";
        return result;
    }

} // namespace fmt
