#ifndef FRACTURED_LANDS_WORLD_HPP_
#define FRACTURED_LANDS_WORLD_HPP_

#include <cstdint>
#include <string>
#include <unordered_map>
#include <list>
#include <vector>
#include "vec4.h"

class Room;
enum room_type : uint8_t;

struct worldgen_info
{
  uint16_t width = 360;
  uint16_t length = 360;
  uint16_t height = 360;
  int octaves = 3;
  int x_offset = 0;
  int y_offset = 0;
  float scale = 30.f;
  float persistance = 0.5f;
  float lacunarity = 2.f;
  float slice = 0.f;
  uint16_t sea_level = 217;
  uint16_t hill_level = 250;
  uint16_t mountain_level = 300;
  uint16_t sky_height = 50;
};

struct mapgen_info
{
  uint16_t width;
  uint16_t height;
};

struct world_map
{
  uint16_t width;
  uint16_t height;
  room_type **tiles;
};

class World
{
public:
  struct terrain_info
  {
    uint16_t height;
    room_type type;
  };

  World(const worldgen_info &world_data = worldgen_info(), const std::vector<mapgen_info> &map_data = {{45,23}});
  ~World();
  vec4 convertRoomIdToCoordinates(uint64_t room_id);
  uint64_t flattenCoordinates(vec4 coordinates);
  Room *getRoom(uint64_t room_id);
  Room *generateRoomInfo(uint64_t room_id);

  uint16_t getWidth() const;
  uint16_t getLength() const;
  uint16_t getHeight() const;
  const world_map *getMap(size_t i) const;

  terrain_info **terrain;

private:
  std::vector<world_map> maps;
  std::unordered_map<uint64_t, Room> rooms;
  uint16_t width;
  uint16_t length;
  uint16_t height;
  uint16_t sea_level;
};

#endif

