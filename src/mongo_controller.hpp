/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#ifndef FRACTURED_LANDS_MONGO_CONTROLLER_HPP_
#define FRACTURED_LANDS_MONGO_CONTROLLER_HPP_

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <string_view>
#include <optional>

class Creature;
class Player;
class Room;

class DatabaseController
{
public:
  DatabaseController();
  ~DatabaseController();

  std::optional<std::string> createPlayer(const Player &player);
  bool readPlayer(std::string_view name, Player &player, Creature &body);
  bool updatePlayer(const Player &player);
  bool deletePlayer(std::string_view name);

  std::optional<std::string> createCreature(const Creature &creature);
  bool readCreature(std::string_view creature_id, Creature &creature);
  bool updateCreature(const Creature &creature);
  bool deleteCreature(std::string_view creature_id);

  bool createRoom(const Room &room);
  bool readRoom(uint64_t room_id);
  bool updateRoom(const Room &room);
  bool deleteRoom(uint64_t room_id);

private:
  mongocxx::instance instance;
  mongocxx::client client;
  mongocxx::database db;

  mongocxx::collection players;
  mongocxx::collection creatures;
  mongocxx::collection rooms;
};

#endif
