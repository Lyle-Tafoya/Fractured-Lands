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
