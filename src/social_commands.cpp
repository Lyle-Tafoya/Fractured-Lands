/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#include "creature.hpp"

void Creature::bowCommand(Creature &creature, std::string_view)
{
  creature.broadcastToRoom(creature.name + " bows gracefully.\r\n");
  creature.receiveMessage("You bow to your audience.\r\n");
}

void Creature::nodCommand(Creature &creature, std::string_view)
{
  creature.broadcastToRoom(creature.name + " nods solemnly.\r\n");
  creature.receiveMessage("You nod solemnly.\r\n");
}
