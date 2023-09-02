/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#include "perlin.hpp"
#include "room.hpp"
#include "world.hpp"

World::World(const worldgen_info &world_data, const std::vector<mapgen_info> &map_data)
{
  width = world_data.width;
  length = world_data.length;
  height = world_data.height;
  sea_level = world_data.sea_level;
  terrain = new terrain_info *[width];
  for(int i = 0; i < width; ++i)
  {
    terrain[i] = new terrain_info[length];
  }
  PerlinNoise noise_generator;
  for(int x = 0; x < width; ++x)
  {
    for(int y = 0; y < length; ++y)
    {
      float amplitude = 1.f;
      float frequency = 1.f;
      float noise_height = 0.f;
      for(int i = 0; i < world_data.octaves; ++i)
      {
        float x_sample = (x+world_data.x_offset) / world_data.scale * frequency;
        float y_sample = (y+world_data.y_offset) / world_data.scale * frequency;
        float n = noise_generator.noise(x_sample, y_sample, world_data.slice);
        noise_height += n * amplitude;
        amplitude *= world_data.persistance;
        frequency *= world_data.lacunarity;
      }
      terrain[x][y].height = (noise_height+1.f)/2.f * (height - world_data.sky_height);
      if(terrain[x][y].height < world_data.sea_level)
      {
        terrain[x][y].type = SEA_BED;
      }
      else if(terrain[x][y].height  < sea_level+4)
      {
        terrain[x][y].type = BEACH;
        float x_sample = static_cast<float>(x + world_data.x_offset) / width;
        float y_sample = static_cast<float>(y + world_data.y_offset) / length;
        float n = noise_generator.noise(x_sample, y_sample, world_data.slice);
        if(n < -0.33f)
        {
          terrain[x][y].type = MARSH;
        }
        else
        {
          terrain[x][y].type = BEACH;
        }
      }
      else if(terrain[x][y].height < world_data.hill_level)
      {
        float x_sample = static_cast<float>(x + world_data.x_offset) / width;
        float y_sample = static_cast<float>(y + world_data.y_offset) / length;
        float n = noise_generator.noise(x_sample, y_sample, world_data.slice);
        if(n < -0.33f)
        {
          terrain[x][y].type = JUNGLE;
        }
        else if(n < 0.33f)
        {
          terrain[x][y].type = FIELD;
        }
        else
        {
          terrain[x][y].type = FOREST;
        }
      }
      else if(terrain[x][y].height < world_data.mountain_level)
      {
        terrain[x][y].type = HILLS;
      }
      else
      {
        terrain[x][y].type = MOUNTAINS;
      }
    }
  }
  for(const auto &map_dimensions : map_data)
  {
    std::unordered_map<room_type, int> terrain_counts;
    int sample_width = width / map_dimensions.width;
    int sample_height = length / map_dimensions.height;
    room_type **tiles = new room_type *[map_dimensions.width];
    for(int i = 0; i < map_dimensions.width; ++i)
    {
      tiles[i] = new room_type[map_dimensions.height];
    }
    for(int y = 0; y < map_dimensions.height; ++y)
    {
      for(int x = 0; x < map_dimensions.width; ++x)
      {
        terrain_counts.clear();
        int start_x = static_cast<float>(width) / map_dimensions.width * x;
        for(int sample_x = start_x; sample_x < start_x+sample_width; ++sample_x)
        {
          int start_y = static_cast<float>(length) / map_dimensions.height * y;
          for(int sample_y = start_y; sample_y < start_y+sample_height; ++sample_y)
          {
            if(!terrain_counts.contains(terrain[sample_x][sample_y].type))
            {
              terrain_counts[terrain[sample_x][sample_y].type] = 1;
            }
            else
            {
              ++terrain_counts[terrain[sample_x][sample_y].type];
            }
          }
        }
        int highest_count = 0;
        room_type highest_count_type = VOID;
        for(const auto &kv : terrain_counts)
        {
          if(kv.second > highest_count)
          {
            highest_count = kv.second;
            highest_count_type = kv.first;
          }
        }
        tiles[x][y] = highest_count_type;
      }
    }
    maps.emplace_back(map_dimensions.width, map_dimensions.height, tiles);
  }
}

const world_map *World::getMap(size_t i) const
{
  return &maps[i];
}

World::~World()
{
  for(int i = 0; i < width; ++i)
  {
    delete[] terrain[i];
  }
  delete[] terrain;

  for(const world_map &m : maps)
  {
    for(int i = 0; i < m.width; ++i)
    {
      delete[] m.tiles[i];
    }
    delete[] m.tiles;
  }
}

vec4 World::convertRoomIdToCoordinates(uint64_t room_id)
{
  return
  {
    static_cast<uint16_t>(room_id%width),
    static_cast<uint16_t>((room_id/width)%length),
    static_cast<uint16_t>(room_id/(width*length)%height),
    static_cast<uint16_t>(room_id/(width*length*height))
  };
}

uint64_t World::flattenCoordinates(vec4 coordinates)
{
  return coordinates.x + coordinates.y*width + coordinates.z*width*length + coordinates.w*width*length*height;
}

Room *World::generateRoomInfo(uint64_t room_id)
{
  vec4 coordinates = convertRoomIdToCoordinates(room_id);
  terrain_info &t = terrain[coordinates.x][coordinates.y];
  room_type terrain_type;
  switch(t.type)
  {
    case HILLS:
      if(coordinates.z < t.height)
      {
        terrain_type = UNDER_GROUND;
      }
      else if(coordinates.z > t.height)
      {
        terrain_type = MID_AIR;
      }
      else
      {
        terrain_type = HILLS;
      }
      break;
    case FOREST:
      if(coordinates.z < t.height)
      {
        terrain_type = UNDER_GROUND;
      }
      else if(coordinates.z > t.height)
      {
        if(coordinates.z == t.height+1)
        {
          terrain_type = TREE_TOPS;
        }
        else
        {
          terrain_type = MID_AIR;
        }
      }
      else
      {
        terrain_type = FOREST;
      }
      break;
    case MOUNTAINS:
      if(coordinates.z < t.height)
      {
        terrain_type = UNDER_GROUND;
      }
      else if(coordinates.z > t.height)
      {
        terrain_type = MID_AIR;
      }
      else
      {
        terrain_type = MOUNTAINS;
      }
      break;
    case MARSH:
      if(coordinates.z < t.height)
      {
        terrain_type = UNDER_GROUND;
      }
      else if(coordinates.z > t.height)
      {
        terrain_type = MID_AIR;
      }
      else
      {
        terrain_type = MARSH;
      }
      break;
    case JUNGLE:
      if(coordinates.z < t.height)
      {
        terrain_type = UNDER_GROUND;
      }
      else if(coordinates.z > t.height)
      {
        if(coordinates.z == t.height+1)
        {
          terrain_type = JUNGLE_CANOPY;
        }
        else
        {
          terrain_type = MID_AIR;
        }
      }
      else
      {
        terrain_type = JUNGLE;
      }
      break;
    case FIELD:
      if(coordinates.z < t.height)
      {
        terrain_type = UNDER_GROUND;
      }
      else if(coordinates.z > t.height)
      {
        terrain_type = MID_AIR;
      }
      else
      {
        terrain_type = FIELD;
      }
      break;
    case BEACH:
      if(coordinates.z < t.height)
      {
        terrain_type = UNDER_GROUND;
      }
      else if(coordinates.z > t.height)
      {
        terrain_type = MID_AIR;
      }
      else
      {
        terrain_type = BEACH;
      }
      break;
    case DESERT:
      if(coordinates.z < t.height)
      {
        terrain_type = UNDER_GROUND;
      }
      else if(coordinates.z > t.height)
      {
        terrain_type = MID_AIR;
      }
      else
      {
        terrain_type = DESERT;
      }
      break;
    case ICE_PLAIN:
      if(coordinates.z < t.height)
      {
        terrain_type = UNDER_GROUND;
      }
      else if(coordinates.z > t.height)
      {
        terrain_type = MID_AIR;
      }
      else
      {
        terrain_type = ICE_PLAIN;
      }
      break;
    case SEA_BED:
      if(coordinates.z < t.height)
      {
        terrain_type = UNDER_GROUND;
      }
      else if(coordinates.z > t.height)
      {
        if(coordinates.z == sea_level)
        {
          terrain_type = WATER_SURFACE;
        }
        else if(coordinates.z == sea_level-1)
        {
          terrain_type = WATER_BELOW_SURFACE;
        }
        else if(coordinates.z < sea_level)
        {
          terrain_type = UNDERWATER;
        }
        else
        {
          terrain_type = MID_AIR;
        }
      }
      else
      {
        terrain_type = SEA_BED;
      }
      break;
    default:
      terrain_type = VOID;
  }
  Room room(room_id, Room::base_rooms[terrain_type].title, Room::base_rooms[terrain_type].description);

  if(coordinates.z == terrain[coordinates.x][coordinates.y].height && coordinates.z >= sea_level)
  {
    uint16_t tmp = coordinates.x == 0 ? width-1 : coordinates.x-1;
    room.exits.emplace_front("west", flattenCoordinates({tmp, coordinates.y, std::max(terrain[tmp][coordinates.y].height, sea_level), coordinates.w}));
    tmp = (coordinates.x+1) % width;
    room.exits.emplace_front("east", flattenCoordinates({tmp, coordinates.y, std::max(terrain[tmp][coordinates.y].height, sea_level), coordinates.w}));
    tmp = (coordinates.y+1) % length;
    room.exits.emplace_front("south", flattenCoordinates({coordinates.x, tmp, std::max(terrain[coordinates.x][tmp].height, sea_level), coordinates.w}));
    tmp = coordinates.y == 0 ? length-1 : coordinates.y-1;
    room.exits.emplace_front("north", flattenCoordinates({coordinates.x, tmp, std::max(terrain[coordinates.x][tmp].height, sea_level), coordinates.w}));
    if(terrain_type == FOREST || terrain_type == JUNGLE)
    {
      room.exits.emplace_front("climb", flattenCoordinates({coordinates.x, coordinates.y, static_cast<uint16_t>(terrain[coordinates.x][coordinates.y].height+1), coordinates.w}));
    }
    else if(terrain_type == SEA_BED)
    {
      room.exits.emplace_front("up", flattenCoordinates({coordinates.x, coordinates.y, static_cast<uint16_t>(terrain[coordinates.x][coordinates.y].height+1), coordinates.w}));
    }
  }
  else
  {
    uint16_t tmp = coordinates.z-1;
    if(tmp >= terrain[coordinates.x][coordinates.y].height)
    {
      room.exits.emplace_front("down", flattenCoordinates({coordinates.x, coordinates.y, tmp, coordinates.w}));
    }
    tmp = coordinates.z+1;
    if(tmp >= terrain[coordinates.x][coordinates.y].height && coordinates.z != sea_level)
    {
      room.exits.emplace_front("up", flattenCoordinates({coordinates.x, coordinates.y, tmp, coordinates.w}));
    }
    tmp = coordinates.x == 0 ? width-1 : coordinates.x-1;
    if(coordinates.z >= terrain[tmp][coordinates.y].height)
    {
      room.exits.emplace_front("west", flattenCoordinates({tmp, coordinates.y, coordinates.z, coordinates.w}));
    }
    tmp = (coordinates.x+1) % width;
    if(coordinates.z >= terrain[tmp][coordinates.y].height)
    {
      room.exits.emplace_front("east", flattenCoordinates({tmp, coordinates.y, coordinates.z, coordinates.w}));
    }
    tmp = (coordinates.y+1) % length;
    if(coordinates.z >= terrain[coordinates.x][tmp].height)
    {
      room.exits.emplace_front("south", flattenCoordinates({coordinates.x, tmp, coordinates.z, coordinates.w}));
    }
    tmp = coordinates.y == 0 ? length-1 : coordinates.y-1;
    if(coordinates.z >= terrain[coordinates.x][tmp].height)
    {
      room.exits.emplace_front("north", flattenCoordinates({coordinates.x, tmp, coordinates.z, coordinates.w}));
    }
  }
  return &(*rooms.insert({room_id, std::move(room)}).first).second;
}

Room *World::getRoom(uint64_t room_id)
{
  Room *room = nullptr;
  const auto &kv = rooms.find(room_id);
  if(kv != rooms.end())
  {
    room = &kv->second;
  }
  else
  {
    room = generateRoomInfo(room_id);
  }
  return room;
}

uint16_t World::getWidth() const
{
  return width;
}
uint16_t World::getLength() const
{
  return length;
}
uint16_t World::getHeight() const
{
  return height;
}
