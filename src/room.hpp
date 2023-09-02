#ifndef FRACTURED_LANDS_ROOM_HPP_
#define FRACTURED_LANDS_ROOM_HPP_

#include <cstdint>
#include <forward_list>
#include <string>
#include <string_view>

class Creature;

enum room_type : uint8_t
{
  VOID = 0,
  HILLS,
  FOREST,
  TREE_TOPS,
  MOUNTAINS,
  MARSH,
  JUNGLE,
  JUNGLE_CANOPY,
  FIELD,
  BEACH,
  DESERT,
  ICE_PLAIN,
  WATER_SURFACE,
  WATER_BELOW_SURFACE,
  UNDERWATER,
  SEA_BED,
  MID_AIR,
  UNDER_GROUND,
  TRAIL,
  TRACK,
  ROAD,
  STRUCTURE,
  DUNGEON
};

class Room
{
public:
  struct exit_info
  {
    const std::string name;
    uint64_t room_id;
  };

  Room(uint64_t id, std::string_view title, std::string_view description);
  uint64_t id;
  std::string_view title;
  std::string_view description;
  std::forward_list<exit_info> exits;
  std::forward_list<Creature *> creatures;

  static const struct base_room_info
  {
    char symbol;
    std::string_view color_code;
    std::string_view title;
    std::string_view description;
  } base_rooms[];
};

#endif
