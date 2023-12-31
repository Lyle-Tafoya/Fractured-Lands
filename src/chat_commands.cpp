/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/

#include "creature.hpp"
#include "colors.hpp"
#include "player.hpp"
#include "server.hpp"
#include "utilities.hpp"
#include <string>
#include <string_view>

void Creature::sayCommand(Creature &creature, std::string_view argument)
{
  std::string name = creature.soul ? creature.soul->getName() : creature.name;
  creature.broadcastToRoom("|G" + name + " says: " + std::string(argument) + "|n\r\n");
  creature.receiveMessage("You say: " + std::string(argument) + "\r\n");
}

void Creature::emoteCommand(Creature &creature, std::string_view argument)
{
  std::string name = creature.soul ? creature.soul->getName() : creature.name;
  creature.broadcastToRoom(name + " " + std::string(argument) + "\r\n");
  creature.receiveMessage("You emote: " + std::string(argument) + "\r\n");
}

// Out-of-character shout
void Creature::oocCommand(Creature &creature, std::string_view argument)
{
  std::string name = creature.soul ? creature.soul->getName() : creature.name;
  creature.broadcastToWorld("|R" + name + " [Out of Character] shouts: " + std::string(argument) + "|n\r\n");
  creature.receiveMessage("You [Out of Character] shout: " + std::string(argument) + "\r\n");
}

// Send a message to everyone on the server
void Creature::shoutCommand(Creature &creature, std::string_view argument)
{
  std::string name = creature.soul ? creature.soul->getName() : creature.name;
  creature.broadcastToWorld("|R" + name + " shouts: " + std::string(argument) + "|n\r\n");
  creature.receiveMessage("You shout: " + std::string(argument) + "\r\n");
}

// Send a private message to another player
void Creature::tellCommand(Creature &creature, std::string_view argument)
{
  std::string target_name;
  std::string message;
  splitStringOnFirst(argument, target_name, message);
  if(target_name.empty() || message.empty())
  {
    creature.receiveMessage("Tell who what?\r\n");
    return;
  }

  auto playerMap = Player::getPlayerNameMap();
  properCase(target_name);
  auto target_player = playerMap.find(target_name);
  if(target_player == playerMap.end())
  {
    creature.receiveMessage("No living thing with that name\r\n");
    return;
  }

  creature.receiveMessage("You tell " + target_name + ": " + message + "\r\n");
  std::string name = creature.soul ? creature.soul->getName() : creature.name;
  target_player->second->receiveMessage("|Y"+name+" tells you: "+message+"|n\r\n");
}
