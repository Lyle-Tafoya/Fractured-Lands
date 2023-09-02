#include "commands.hpp"
#include "creature.hpp"
#include "player.hpp"
#include "room.hpp"
#include "world.hpp"
  extern World world;
#include <set>

bool Creature::doExitCommand(const std::string &command, const std::string &)
{
  for(const Room::exit_info &room_exit : room->exits)
  {
    if(command == room_exit.name)
    {
      broadcastToRoom(name + " leaves " + command + ".\r\n");
      move(room_exit.room_id);
      return true;
    }
  }
  return false;
}

const std::set<std::string> known_exits =
{
  {"north"},
  {"east"},
  {"south"},
  {"west"},
  {"up"},
  {"down"},
  {"northeast"},
  {"southeast"},
  {"southwest"},
  {"northwest"},
  {"cliff-face-north"},
  {"cliff-face-east"},
  {"cliff-face-south"},
  {"cliff-face-west"},
  {"enter"},
  {"stairs"}
};

bool Creature::doCommand(const std::string &command, const std::string &argument)
{

  bool known_exit_command = known_exits.contains(command);
  if(doExitCommand(command, argument))
  {
    return true;
  }

  auto commandFunc = commandMap.find(command);
  if(commandFunc != commandMap.end())
  {
    commandFunc->second(*this, argument);
    return true;
  }
  else
  {
    if(known_exit_command)
    {
      receiveMessage("There is no exit to the " + command + ".\r\n");
    }
    else
    {
      receiveMessage("What?\r\n");
    }
    return false;
  }
}

void Creature::broadcastToRoom(const std::string &message)
{
  for(const auto &creature : room->creatures)
  {
    if(creature->soul && (!soul || creature->soul->getFileDescriptor() != soul->getFileDescriptor()))
    {
      creature->soul->receiveMessage(message);
    }
  }
}

void Creature::broadcastToWorld(const std::string &message)
{
  for(const auto &it : Player::getPlayerFDMap())
  {
    if(!soul || it.first != soul->getFileDescriptor())
    {
      it.second->receiveMessage(message);
    }
  }
}

void Creature::receiveMessage(const std::string &message)
{
  if(soul)
  {
    soul->receiveMessage(message);
  }
}

void Creature::move(uint64_t room_id)
{
  room->creatures.remove(this);
  room = world.getRoom(room_id);
  Player::lookCommand(*this, "");
  room->creatures.emplace_front(this);
  broadcastToRoom(name + " arrives.\r\n");
}

void Creature::recalculateHp()
{
  auto tmp = max_hp;
  max_hp = strength*2 + constitution*6 + size + 20;
  hp = tmp ? hp*100/tmp*max_hp/100 : max_hp;
}

void Creature::recalculateSp()
{
  auto tmp = max_sp;
  max_sp = intelligence + power*8 + 20;
  sp = tmp ? sp*100/tmp*max_sp/100 : max_sp;
}

void Creature::setRoom(uint64_t room_id)
{
  room = world.getRoom(room_id);
}

void Creature::setName(std::string_view name)
{
  this->name = name;
}

void Creature::setStrength(int16_t amount)
{
  strength = amount;
  recalculateHp();
}
void Creature::setDexterity(int16_t amount)
{
  dexterity = amount;
}
void Creature::setConstitution(int16_t amount)
{
  constitution = amount;
  recalculateHp();
}
void Creature::setIntelligence(int16_t amount)
{
  intelligence = amount;
  recalculateSp();
}
void Creature::setWisdom(int16_t amount)
{
  wisdom = amount;
}
void Creature::setPower(int16_t amount)
{
  power = amount;
  recalculateSp();
}
void Creature::setSanity(int16_t amount)
{
  sanity = amount;
}
void Creature::setSize(int16_t amount)
{
  size = amount;
  recalculateHp();
}
void Creature::setChutzpah(int16_t amount)
{
  chutzpah = amount;
}
void Creature::setId(std::string_view id)
{
  this->id = id;
}

int16_t Creature::getChutzpah() const
{
  return chutzpah;
}
int16_t Creature::getConstitution() const
{
  return constitution;
}
int16_t Creature::getDexterity() const
{
  return dexterity;
}
int16_t Creature::getHp() const
{
  return hp;
}
int16_t Creature::getIntelligence() const
{
  return intelligence;
}
int16_t Creature::getMaxHp() const
{
  return max_hp;
}
int16_t Creature::getMaxSp() const
{
  return max_sp;
}
int16_t Creature::getPower() const
{
  return power;
}
int16_t Creature::getSanity() const
{
  return sanity;
}
int16_t Creature::getSize() const
{
  return size;
}
int16_t Creature::getSp() const
{
  return sp;
}
int16_t Creature::getStrength() const
{
  return strength;
}
int16_t Creature::getWisdom() const
{
  return wisdom;
}
int16_t Creature::getSatiation() const
{
  return satiation;
}
int16_t Creature::getIntoxication() const
{
  return intoxication;
}
Creature::combat_action Creature::getAction() const
{
  return action;
}
Creature::combat_action Creature::getReaction() const
{
  return reaction;
}
const std::string &Creature::getId() const
{
  return id;
}
const Room *Creature::getRoom() const
{
  return room;
}

const std::string &Creature::getName() const
{
  return name;
}

void Creature::setHp(int16_t value)
{
  hp = value;
}

void Creature::setSp(int16_t value)
{
  sp = value;
}

void Creature::setMaxHp(int16_t value)
{
  max_hp = value;
}

void Creature::setMaxSp(int16_t value)
{
  max_sp = value;
}

void Creature::setAction(Creature::combat_action action)
{
  this->action = action;
}

void Creature::setReaction(Creature::combat_action reaction)
{
  this->reaction = reaction;
}

void Creature::setSatiation(int16_t value)
{
  this->satiation = value;
}
void Creature::setIntoxication(int16_t value)
{
  this->intoxication = value;
}
