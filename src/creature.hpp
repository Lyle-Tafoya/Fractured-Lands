#ifndef FRACTURED_LANDS_CREATURE_HPP_
#define FRACTURED_LANDS_CREATURE_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include "player.hpp"
  class Player;
#include "room.hpp"
  class Room;

class Creature
{
public:
  enum combat_action : uint8_t { ATTACK, PARRY, DODGE };

  const std::string &getId() const;
  int16_t getChutzpah() const;
  int16_t getConstitution() const;
  int16_t getDexterity() const;
  int16_t getIntelligence() const;
  int16_t getPower() const;
  int16_t getSanity() const;
  int16_t getSize() const;
  int16_t getStrength() const;
  int16_t getWisdom() const;
  int16_t getHp() const;
  int16_t getMaxHp() const;
  int16_t getSp() const;
  int16_t getMaxSp() const;
  int16_t getSatiation() const;
  int16_t getIntoxication() const;
  combat_action getAction() const;
  combat_action getReaction() const;
  const Room *getRoom() const;
  const std::string &getName() const;

  void setChutzpah(int16_t amount);
  void setConstitution(int16_t amount);
  void setDexterity(int16_t amount);
  void setIntelligence(int16_t amount);
  void setPower(int16_t amount);
  void setSanity(int16_t amount);
  void setSize(int16_t amount);
  void setStrength(int16_t amount);
  void setWisdom(int16_t amount);
  void setHp(int16_t amount);
  void setMaxHp(int16_t amount);
  void setSp(int16_t amount);
  void setMaxSp(int16_t amount);
  void setName(std::string_view name);
  void setId(std::string_view id);
  void setRoom(uint64_t room_id);
  void setAction(combat_action action);
  void setReaction(combat_action action);
  void setSatiation(int16_t);
  void setIntoxication(int16_t);


  void receiveMessage(const std::string &message);
  void broadcastToRoom(const std::string &message);
  void broadcastToWorld(const std::string &message);

  bool doCommand(const std::string &command, const std::string &argument);
  bool doExitCommand(const std::string &command, const std::string &argument);

  // chat_commands.cpp
  static void oocCommand(Creature &creature, std::string_view argument);
  static void shoutCommand(Creature &creature, std::string_view argument);
  static void tellCommand(Creature &creature, std::string_view argument);
  static void emoteCommand(Creature &creature, std::string_view argument);
  static void sayCommand(Creature &creature, std::string_view argument);

  // social_commands.cpp
  static void bowCommand(Creature &creature, std::string_view argument);
  static void nodCommand(Creature &creature, std::string_view argument);

  // combat_commands.cpp
  static void actionCommand(Creature &creature, std::string_view argument);
  static void reactionCommand(Creature &creature, std::string_view argument);

  // movement_commands.cpp
  static void northCommand(Creature &creature, std::string_view);
  static void eastCommand(Creature &creature, std::string_view);
  static void southCommand(Creature &creature, std::string_view);
  static void westCommand(Creature &creature, std::string_view);
  static void northeastCommand(Creature &creature, std::string_view);
  static void southeastCommand(Creature &creature, std::string_view);
  static void southwestCommand(Creature &creature, std::string_view);
  static void northwestCommand(Creature &creature, std::string_view);
  static void upCommand(Creature &creature, std::string_view);
  static void downCommand(Creature &creature, std::string_view);
  static void cliffFaceNorthCommand(Creature &creature, std::string_view);
  static void cliffFaceEastCommand(Creature &creature, std::string_view);
  static void cliffFaceSouthCommand(Creature &creature, std::string_view);
  static void cliffFaceWestCommand(Creature &creature, std::string_view);
  static void cliffDropNorthCommand(Creature &creature, std::string_view);
  static void cliffDropEastCommand(Creature &creature, std::string_view);
  static void cliffDropSouthCommand(Creature &creature, std::string_view);
  static void cliffDropWestCommand(Creature &creature, std::string_view);

protected:
  Player *soul = nullptr;

private:
  struct body_part
  {
    enum status : uint8_t { NORMAL, SPEARED, CRUSHED, SEVERED, IMPLODED };
    status condition = NORMAL;
    bool vital;
    int16_t max_hp;
    int16_t hp;
    std::string name;
  };
  std::vector<body_part> body_parts;

  combat_action action = ATTACK;
  combat_action reaction = PARRY;
  int16_t intoxication = 0;
  int16_t satiation = 0;

  int16_t chutzpah = 0;
  int16_t constitution = 0;
  int16_t dexterity = 0;
  int16_t intelligence = 0;
  int16_t power = 0;
  int16_t sanity = 0;
  int16_t size = 0;
  int16_t strength = 0;
  int16_t wisdom = 0;

  int16_t hp = 20;
  int16_t max_hp = 20;
  int16_t sp = 20;
  int16_t max_sp = 20;

  Room *room = nullptr;

  std::string id;
  std::string name;
  std::unordered_map<std::string, size_t> skills;

  void move(uint64_t room_id);

  void recalculateHp();
  void recalculateSp();

  static const std::string_view dexterity_descriptions[];
  static const std::string_view power_descriptions[];
  static const std::string_view intelligence_descriptions[];
  static const std::string_view sanity_descriptions[];
  static const std::string_view chutzpah_descriptions[];
  static const std::string_view wisdom_descriptions[];
  static const std::string_view strength_descriptions[];
  static const std::string_view size_descriptions[];
  static const std::string_view constitution_descriptions[];

  friend class Player;
};

#endif
