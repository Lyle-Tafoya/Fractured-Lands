#include <fmt/core.h>
#include "player.hpp"
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
