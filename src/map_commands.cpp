/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#include "colors.hpp"
#include "creature.hpp"
#include "player.hpp"
#include "room.hpp"
#include "world.hpp"
#include "vec4.h"
#include <string>
#include <string_view>

extern World world;

void Player::worldmapCommand(Creature &creature, std::string_view)
{
  if(!creature.soul)
  {
    return;
  }
  const world_map *map_data = world.getMap(0);
  if(!map_data)
  {
    return;
  }
  vec4 coordinates = world.convertRoomIdToCoordinates(creature.room->id);
  int scaled_x = static_cast<float>(coordinates.x) / world.getWidth() * map_data->width;
  int scaled_y = static_cast<float>(coordinates.y) / world.getHeight() * map_data->height;
  std::string worldmap;
  for(int y = 0; y < map_data->height; ++y)
  {
    for(int x = 0; x < map_data->width; ++x)
    {
      if(x == scaled_x && y == scaled_y)
      {
        worldmap += COLOR_BRIGHT_MAGENTA;
        worldmap.push_back('@');
      }
      else
      {
        worldmap += Room::base_rooms[map_data->tiles[x][y]].color_code;
        worldmap.push_back(Room::base_rooms[map_data->tiles[x][y]].symbol);
      }
    }
    worldmap += "\r\n";
  }
  creature.soul->receiveMessage(worldmap + "\r\n"
    "     |G+|n - |cforest  |Gl|n - |cfield  |g%|n - |cjungle  |g&|n - |cmarsh |K^|n - |cmountains\r\n"
    "     |Y&|n - |cdesert  |B~|n - |cwater  |G^|n - |chills   |Y%|n - |cbeach |WI|n - |cice plains\r\n");
}

void Player::areamapCommand(Creature &creature, std::string_view)
{
  if(!creature.soul)
  {
    return;
  }
  const world_map *map_data = world.getMap(1);
  if(!map_data)
  {
    return;
  }
  vec4 coordinates = world.convertRoomIdToCoordinates(creature.room->id);
  int scaled_x = static_cast<float>(coordinates.x) / world.getWidth() * map_data->width;
  int scaled_y = static_cast<float>(coordinates.y) / world.getHeight() * map_data->height;
  std::string areamap;
  for(int y = scaled_y-3; y <= scaled_y+3; ++y)
  {
    for(int x = scaled_x-6; x <= scaled_x+6; ++x)
    {
      if(x == scaled_x && y == scaled_y)
      {
        areamap += COLOR_BRIGHT_MAGENTA;
        areamap .push_back('@');
      }
      else
      {
        areamap += Room::base_rooms[map_data->tiles[x][y]].color_code;
        areamap.push_back(Room::base_rooms[map_data->tiles[x][y]].symbol);
      }
    }
    areamap += "\r\n";
  }
  creature.soul->receiveMessage(areamap);
}

void Player::minimapCommand(Creature &creature, std::string_view)
{
  if(!creature.soul)
  {
    return;
  }
  std::string minimap;
  vec4 coordinates = world.convertRoomIdToCoordinates(creature.room->id);
  for(int y = coordinates.y-4; y <= coordinates.y+4; ++y)
  {
    for(int x = coordinates.x-7; x <= coordinates.x+7; ++x)
    {
      if(x == coordinates.x && y == coordinates.y)
      {
        minimap += COLOR_BRIGHT_MAGENTA;
        minimap.push_back('@');
      }
      else
      {
        minimap += Room::base_rooms[world.terrain[x][y].type].color_code;
        minimap.push_back(Room::base_rooms[world.terrain[x][y].type].symbol);
      }
    }
    minimap += "\r\n";
  }
  minimap += "|n";
  creature.soul->receiveMessage(minimap);
}
