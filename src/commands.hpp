#ifndef FRACTURED_LANDS_COMMANDS_HPP_
#define FRACTURED_LANDS_COMMANDS_HPP_

#include <functional>
#include <unordered_map>
#include <string>

#include "creature.hpp"

extern std::unordered_map<std::string, std::function<void(Creature &, std::string_view)>> commandMap;

#endif
