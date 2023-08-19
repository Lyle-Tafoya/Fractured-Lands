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
