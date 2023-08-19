#ifndef FRACTURED_LANDS_MONGO_CONTROLLER_HPP_
#define FRACTURED_LANDS_MONGO_CONTROLLER_HPP_

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <string_view>
#include <optional>
#include "player.hpp"
#include "creature.hpp"

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
