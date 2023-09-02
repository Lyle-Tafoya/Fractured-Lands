/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#ifndef FRACTURED_LANDS_CONNECTION_INFO_HPP_
#define FRACTURED_LANDS_CONNECTION_INFO_HPP_

#include <queue>
#include <string>
#include <unordered_map>

class Player;

class ConnectionInfo
{
public:
  int fd;
  std::string inputBuffer;
  std::queue<std::string> inputQueue;
  Player *player;

  ConnectionInfo(int fd);
  ~ConnectionInfo();

  int getFileDescriptor() const;
  void processInput(std::string const& input);

  static std::unordered_map<int,ConnectionInfo *> lookupConnectionInfoByFileDescriptor;
};

#endif
