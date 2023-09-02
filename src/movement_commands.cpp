/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#include "creature.hpp"

void Creature::northCommand(Creature &creature, std::string_view)
{
  creature.doCommand("north", "");
}
void Creature::eastCommand(Creature &creature, std::string_view)
{
  creature.doCommand("east", "");
}
void Creature::southCommand(Creature &creature, std::string_view)
{
  creature.doCommand("south", "");
}
void Creature::westCommand(Creature &creature, std::string_view)
{
  creature.doCommand("west", "");
}
void Creature::northeastCommand(Creature &creature, std::string_view)
{
  creature.doCommand("northeast", "");
}
void Creature::southeastCommand(Creature &creature, std::string_view)
{
  creature.doCommand("southeast", "");
}
void Creature::southwestCommand(Creature &creature, std::string_view)
{
  creature.doCommand("southwest", "");
}
void Creature::northwestCommand(Creature &creature, std::string_view)
{
  creature.doCommand("northwest", "");
}
void Creature::upCommand(Creature &creature, std::string_view)
{
  creature.doCommand("up", "");
}
void Creature::downCommand(Creature &creature, std::string_view)
{
  creature.doCommand("down", "");
}
void Creature::cliffFaceNorthCommand(Creature &creature, std::string_view)
{
  creature.doCommand("cliff-face-north", "");
}
void Creature::cliffFaceEastCommand(Creature &creature, std::string_view)
{
  creature.doCommand("cliff-face-east", "");
}
void Creature::cliffFaceSouthCommand(Creature &creature, std::string_view)
{
  creature.doCommand("cliff-face-south", "");
}
void Creature::cliffFaceWestCommand(Creature &creature, std::string_view)
{
  creature.doCommand("cliff-face-west", "");
}
void Creature::cliffDropNorthCommand(Creature &creature, std::string_view)
{
  creature.doCommand("cliff-drop-north", "");
}
void Creature::cliffDropEastCommand(Creature &creature, std::string_view)
{
  creature.doCommand("cliff-drop-east", "");
}
void Creature::cliffDropSouthCommand(Creature &creature, std::string_view)
{
  creature.doCommand("cliff-drop-south", "");
}
void Creature::cliffDropWestCommand(Creature &creature, std::string_view)
{
  creature.doCommand("cliff-drop-west", "");
}
