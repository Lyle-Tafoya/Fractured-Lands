#ifndef FRACTURED_LANDS_CONNECTION_INFO_HPP_
#define FRACTURED_LANDS_CONNECTION_INFO_HPP_

#include <queue>
#include "player.hpp"

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
