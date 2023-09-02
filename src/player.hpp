/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#ifndef FRACTURED_LANDS_PLAYER_HPP_
#define FRACTURED_LANDS_PLAYER_HPP_

#include <unordered_map>
#include <cstdint>
#include <list>
#include <string>

class Creature;
class ConnectionInfo;

class Player
{
public:
  enum states : uint8_t { COLOR_CHECK, USERNAME, NEW_PASSWORD, EXISTING_PASSWORD, PLAYING };

  Player(std::string const& name, ConnectionInfo *connection);
  ~Player();

  bool load();
  bool save();

  int getFileDescriptor() const;
  bool getColorEnabled() const;
  bool getVerbose() const;
  const std::string &getPassword() const;
  const std::string &getName() const;
  const std::string &getPrompt() const;
  const Creature *getBody() const;

  void setColorEnabled(bool color_enabled = true);
  void setVerbose(bool verbose = true);
  void setPassword(std::string_view password);
  void setPrompt(std::string_view prompt);

  void enterWorld();
  void receiveMessage(const std::string &message) const;

  static std::unordered_map<int, Player *> const &getPlayerFDMap();
  static std::unordered_map<std::string, Player *> const &getPlayerNameMap();
  static void handleClosePlayerConnection(int fd);
  static void handleExistingPlayerConnection(int fd, std::string input);
  static void handleNewPlayerConnection(int fd);

  // information_commands.cpp
  static void lookCommand(Creature &creature, std::string_view argument);
  static void scoreCommand(Creature &creature, std::string_view argument);
  static void briefCommand(Creature &creature, std::string_view argument);
  static void promptCommand(Creature &creature, std::string_view argument);
  static void whoCommand(Creature &creature, std::string_view);

  // map_commands.cpp
  static void areamapCommand(Creature &creature, std::string_view argument);
  static void worldmapCommand(Creature &creature, std::string_view argument);
  static void minimapCommand(Creature &creature, std::string_view argument);

  // utility_commands.cpp
  static void saveCommand(Creature &creature, std::string_view argument);
  static void quitCommand(Creature &creature, std::string_view argument);

private:
  states state = USERNAME;
  bool color_enabled = true;
  bool verbose = true;
  ConnectionInfo *connection = nullptr;
  Creature *body = nullptr;
  Creature *primaryBody = nullptr;
  std::string name;
  std::string title = "the newcomer";
  std::string password;
  std::string prompt = "|K[|nHP=%c|n/|G%H |nSP=%C|n/|G%S|K]";
  std::unordered_map<std::string,std::string> aliases;
  uint32_t wizardLevel = 0;
  bool isBountied = false;
  bool isPlayerKiller = false;

  void showPrompt() const;

  static void handleColorCheckInput(Player &player);
  static void handleGameInput(Player &player);
  static void handleNewPasswordInput(Player &player);
  static void handleExistingPasswordInput(Player &player);
  static void handleUsernameInput(ConnectionInfo &connection);
  static std::unordered_map<int, Player *> lookupPlayerByFileDescriptor;
  static std::unordered_map<std::string, Player *> lookupPlayerByName;
  static std::string playerWelcomeText;
  static std::list<Player *> immortals;
  static std::list<Player *> lords;
  static std::list<Player *> guildLeaders;
  static std::list<Player *> peasants;

  static const std::unordered_map<std::string,std::string> prompt_templates;
};

#endif
