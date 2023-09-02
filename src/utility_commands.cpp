/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#include <fmt/core.h>
#include "player.hpp"
#include "creature.hpp"
#include "room.hpp"
#include "server.hpp"

extern Server server;

void Player::saveCommand(Creature &creature, std::string_view)
{
  if(!creature.soul)
  {
    return;
  }
  creature.soul->save();
  creature.soul->receiveMessage("Ok.\r\n");
}

void Player::quitCommand(Creature &creature, std::string_view)
{
  if(!creature.soul)
  {
    return;
  }
  creature.soul->save();
  creature.broadcastToRoom(fmt::format("{} exits the game.\r\n", creature.soul->name));
  creature.room->creatures.remove(&creature);
  server.closeConnection(creature.soul->getFileDescriptor());
}
