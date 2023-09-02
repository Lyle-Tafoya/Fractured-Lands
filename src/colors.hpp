/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#ifndef FRACTURED_LANDS_COLORS_HPP_
#define FRACTURED_LANDS_COLORS_HPP_

#include <string>
#include <string_view>
#include <unordered_map>

constexpr std::string_view COLOR_NORMAL         = "\x1B[0m";
constexpr std::string_view COLOR_BLACK          = "\x1B[0;30m";
constexpr std::string_view COLOR_BRIGHT_BLACK   = "\x1B[0;1;30m";
constexpr std::string_view COLOR_FAINT_BLACK    = "\x1B[0;2;30m";
constexpr std::string_view COLOR_RED            = "\x1B[0;31m";
constexpr std::string_view COLOR_BRIGHT_RED     = "\x1B[0;1;31m";
constexpr std::string_view COLOR_FAINT_RED      = "\x1B[0;2;31m";
constexpr std::string_view COLOR_GREEN          = "\x1B[0;32m";
constexpr std::string_view COLOR_BRIGHT_GREEN   = "\x1B[0;1;32m";
constexpr std::string_view COLOR_FAINT_GREEN    = "\x1B[0;2;32m";
constexpr std::string_view COLOR_YELLOW         = "\x1B[0;33m";
constexpr std::string_view COLOR_BRIGHT_YELLOW  = "\x1B[0;1;33m";
constexpr std::string_view COLOR_FAINT_YELLOW   = "\x1B[0;2;33m";
constexpr std::string_view COLOR_BLUE           = "\x1B[0;34m";
constexpr std::string_view COLOR_BRIGHT_BLUE    = "\x1B[0;1;34m";
constexpr std::string_view COLOR_FAINT_BLUE     = "\x1B[0;2;34m";
constexpr std::string_view COLOR_MAGENTA        = "\x1B[0;35m";
constexpr std::string_view COLOR_BRIGHT_MAGENTA = "\x1B[0;1;35m";
constexpr std::string_view COLOR_FAINT_MAGENTA  = "\x1B[0;2;35m";
constexpr std::string_view COLOR_CYAN           = "\x1B[0;36m";
constexpr std::string_view COLOR_BRIGHT_CYAN    = "\x1B[0;1;36m";
constexpr std::string_view COLOR_FAINT_CYAN     = "\x1B[0;2;36m";
constexpr std::string_view COLOR_WHITE          = "\x1B[0;37m";
constexpr std::string_view COLOR_BRIGHT_WHITE   = "\x1B[0;1;37m";
constexpr std::string_view COLOR_FAINT_WHITE    = "\x1B[0;2;37m";

extern const std::unordered_map<char,std::string_view> COLOR_MAP;
extern void colorizeText(std::string &str);

#endif
