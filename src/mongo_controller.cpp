/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#include <filesystem>
#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <cstring>
#include <bsoncxx/builder/stream/document.hpp>
#include "mongo_controller.hpp"
#include "creature.hpp"
#include "player.hpp"
#include "room.hpp"

extern std::shared_ptr<spdlog::logger> logger;

DatabaseController::DatabaseController()
  : client(mongocxx::uri{"mongodb://localhost:27017"})
{
  db = client["fractured_lands"];
  players = db["players"];
  creatures = db["creatures"];
  rooms = db["rooms"];
}

DatabaseController::~DatabaseController()
{
}

bsoncxx::document::value to_document(const Creature &creature)
{
  return (bsoncxx::builder::stream::document{}
    << "name" << creature.getName()
    << "room_id" << std::to_string(creature.getRoom()->id)
    << "chutzpah" << creature.getChutzpah()
    << "constitution" << creature.getConstitution()
    << "dexterity" << creature.getDexterity()
    << "intelligence" << creature.getIntelligence()
    << "power" << creature.getPower()
    << "sanity" << creature.getSanity()
    << "size" << creature.getSize()
    << "strength" << creature.getStrength()
    << "wisdom" << creature.getWisdom()
    << "hp" << creature.getHp()
    << "max_hp" << creature.getMaxHp()
    << "sp" << creature.getSp()
    << "max_sp" << creature.getMaxSp()
    << "intoxication" << creature.getIntoxication()
    << "satiation" << creature.getSatiation()
    << "combat_action" << creature.getAction()
    << "combat_reaction" << creature.getReaction()
    << bsoncxx::builder::stream::finalize);
}
bsoncxx::document::value to_document(const Player &player)
{
  return (bsoncxx::builder::stream::document{}
    << "_id" << player.getName()
    << "creature_id" << player.getBody()->getId()
    << "password" << player.getPassword()
    << "prompt" << player.getPrompt()
    << "color" << player.getColorEnabled()
    << "verbose" << player.getVerbose()
    << bsoncxx::builder::stream::finalize
  );
}

std::optional<std::string> DatabaseController::createCreature(const Creature &creature)
{
  logger->info("Creating new creature \"{}#{}\" in DB", creature.getName(), creature.getId());
  auto result = creatures.insert_one(to_document(creature));
  if(!result)
  {
    logger->error("Failed to create creature \"{}#{}\" in DB", creature.getName(), creature.getId());
    return std::optional<std::string>(); 
  }
  return std::optional<std::string>(result.value().inserted_id().get_oid().value.to_string());
}
bool DatabaseController::readCreature(std::string_view creature_id, Creature &creature)
{
  logger->info("Reading creature \"{}\" from DB", creature_id);
  auto result = creatures.find_one(bsoncxx::builder::stream::document{}
      << "_id" << bsoncxx::oid(std::string(creature_id))
      << bsoncxx::builder::stream::finalize
  );
  if(!result)
  {
    logger->info("Creature \"{}\" not found in DB", creature_id);
    return false;
  }
  bsoncxx::document::view document_view = result.value().view();
  creature.setId(document_view["_id"].get_oid().value.to_string());
  creature.setName(document_view["name"].get_string());
  creature.setChutzpah(document_view["chutzpah"].get_int32());
  creature.setConstitution(document_view["constitution"].get_int32());
  creature.setDexterity(document_view["dexterity"].get_int32());
  creature.setIntelligence(document_view["intelligence"].get_int32());
  creature.setPower(document_view["power"].get_int32());
  creature.setSanity(document_view["sanity"].get_int32());
  creature.setSize(document_view["size"].get_int32());
  creature.setStrength(document_view["strength"].get_int32());
  creature.setWisdom(document_view["wisdom"].get_int32());
  creature.setIntoxication(document_view["intoxication"].get_int32());
  creature.setSatiation(document_view["satiation"].get_int32());
  creature.setMaxHp(document_view["max_hp"].get_int32());
  creature.setHp(document_view["hp"].get_int32());
  creature.setMaxSp(document_view["max_sp"].get_int32());
  creature.setSp(document_view["sp"].get_int32());
  creature.setAction(static_cast<Creature::combat_action>(static_cast<uint8_t>(document_view["combat_action"].get_int32())));
  creature.setReaction(static_cast<Creature::combat_action>(static_cast<uint8_t>(document_view["combat_reaction"].get_int32())));
  creature.setRoom(std::stoull(std::string(document_view["room_id"].get_string())));
  return true;
}
bool DatabaseController::updateCreature(const Creature &creature)
{
  logger->info("Updating creature \"{}#{}\" in DB", creature.getName(), creature.getId());
  auto result = creatures.replace_one(
    bsoncxx::builder::stream::document{}
      << "_id" << bsoncxx::oid(creature.getId())
      << bsoncxx::builder::stream::finalize,
    to_document(creature)
  );
  if(!result)
  {
    logger->error("Failed to update creature \"{}#{}\" in DB", creature.getName(), creature.getId());
    return false;
  }
  return true;
}
bool DatabaseController::deleteCreature(std::string_view creature_id)
{
  logger->info("Deleting creature \"{}\" in DB", creature_id);
  auto result = creatures.delete_one(bsoncxx::builder::stream::document{}
      << "_id" << std::string(creature_id)
      << bsoncxx::builder::stream::finalize
  );
  if(!result)
  {
    logger->error("Failed to delete creature \"{}\" from DB", creature_id);
    return false;
  }
  return true;
}

std::optional<std::string> DatabaseController::createPlayer(const Player &player)
{
  logger->info("Creating new player \"{}\" in DB", player.getName());
  auto result = players.insert_one(to_document(player));
  if(!result)
  {
    logger->error("Failed to create player \"{}\" in DB", player.getName());
    return std::optional<std::string>();
  }
  return std::optional<std::string>(result.value().inserted_id().get_string());
}

bool DatabaseController::readPlayer(std::string_view name, Player &player, Creature &body)
{
  logger->info("Reading player \"{}\" from DB", name);
  auto result = players.find_one(bsoncxx::builder::stream::document{}
      << "_id" << std::string(name)
      << bsoncxx::builder::stream::finalize
  );
  if(!result)
  {
    logger->info("Player \"{}\" not found in DB", name);
    return false;
  }
  bsoncxx::document::view document_view = result.value().view();
  player.setPassword(document_view["password"].get_string());
  player.setPrompt(document_view["prompt"].get_string());
  player.setColorEnabled(document_view["color"].get_bool());
  player.setVerbose(document_view["verbose"].get_bool());

  std::string creature_id(document_view["creature_id"].get_string());
  return readCreature(creature_id, body);
}
bool DatabaseController::updatePlayer(const Player &player)
{
  logger->info("Updating player \"{}\" in DB", player.getName());
  auto result = players.replace_one(
    bsoncxx::builder::stream::document{}
      << "_id" << player.getName()
      << bsoncxx::builder::stream::finalize,
    to_document(player)
  );
  if(!result)
  {
    logger->error("Failed to update Player \"{}\" in DB", player.getName());
    return false;
  }
  const Creature *body = player.getBody();
  if(body)
  {
    return updateCreature(*body);
  }
  return true;
}
bool DatabaseController::deletePlayer(std::string_view name)
{
  logger->info("Deleting player \"{}\" in DB", name);
  auto result = players.delete_one(bsoncxx::builder::stream::document{}
      << "_id" << std::string(name)
      << bsoncxx::builder::stream::finalize
  );
  if(!result)
  {
    logger->error("Failed to delete Player \"{}\" from DB", name);
    return false;
  }
  return true;
}
