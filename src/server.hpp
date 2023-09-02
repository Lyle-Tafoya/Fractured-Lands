/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#ifndef FRACTURED_LANDS_SERVER_HPP_
#define FRACTURED_LANDS_SERVER_HPP_

#include <functional>
#include <sys/epoll.h>
#include <string>

class Server
{
public:
  Server();
  void closeConnection(int fd);
  void registerCloseConnectionCallback(std::function<void(int)> method);
  void registerExistingConnectionCallback(std::function<void(int, std::string const)> method);
  void registerNewConnectionCallback(std::function<void(int)> method);
  void shutdown();
  bool start(unsigned int port);

  static int sendMessage(int fd, const std::string &message);

private:
  int epollfd;
  epoll_event ev;
  bool running;
  std::function<void(int)> handleNewConnection;
  std::function<void(int)> handleCloseConnection;
  std::function<void(int, std::string const)> handleExistingConnection;
};

#endif
