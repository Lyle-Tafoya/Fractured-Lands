/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#include "creature.hpp"
#include <optional>
#include <string_view>

const std::string_view action_string(Creature::combat_action a)
{
  switch(a)
  {
    case Creature::ATTACK:
      return "attack";
    case Creature::PARRY:
      return "parry";
    case Creature::DODGE:
      return "dodge";
    default:
      return "";
  }
}

std::optional<Creature::combat_action> str_to_action(std::string_view str)
{
  if(str == "attack")
  {
    return std::optional{Creature::ATTACK};
  }
  else if(str == "parry")
  {
    return std::optional{Creature::PARRY};
  }
  else if(str == "dodge")
  {
    return std::optional{Creature::DODGE};
  }
  return std::optional<Creature::combat_action>();
}

void Creature::actionCommand(Creature &creature, std::string_view argument)
{
  if(argument.empty())
  {
    creature.receiveMessage("Your current action is: " + std::string(action_string(creature.action)) + ".\r\n");
  }
  else
  {
    std::optional a = str_to_action(argument);
    if(!a.has_value())
    {
      creature.receiveMessage("That is not a legal action.\r\n");
    }
    else
    {
      creature.action = a.value();
      creature.receiveMessage("Action set to " + std::string(argument) + ".\r\n");
    }
  }
}

void Creature::reactionCommand(Creature &creature, std::string_view argument)
{
  if(argument.empty())
  {
    creature.receiveMessage("Your current reaction is: " + std::string(action_string(creature.reaction)) + ".\r\n");
  }
  else
  {
    std::optional a = str_to_action(argument);
    if(!a.has_value())
    {
      creature.receiveMessage("That is not a legal action.\r\n");
    }
    else
    {
      creature.reaction = a.value();
      creature.receiveMessage("Reaction set to " + std::string(argument) + ".\r\n");
    }
  }
}
