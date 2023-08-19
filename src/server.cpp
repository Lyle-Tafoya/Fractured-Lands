#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "server.hpp"

#define MAX_EVENTS 255
#define BACKLOG 255
#define FD_BUFFER_SIZE 255

Server::Server() : running(false) {}

void Server::closeConnection(int fd)
{
  epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
  close(fd);
  handleCloseConnection(fd);
}

void Server::registerExistingConnectionCallback(std::function<void(int, std::string const)> method)
{
  handleExistingConnection = method;
}

void Server::registerNewConnectionCallback(std::function<void(int)> method)
{
  handleNewConnection = method;
}

void Server::registerCloseConnectionCallback(std::function<void(int)> method)
{
  handleCloseConnection = method;
}

void Server::shutdown()
{
  running = false;
}

bool Server::start(unsigned int port)
{
  if(running == true)
  {
    return false;
  }

  addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  addrinfo *servinfo;
  int status = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &servinfo);
  if(status != 0)
  {
    perror("getaddrinfo");
    return false;
  }

  // Create a socket and set it to listen on the specified port
  int listen_sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  if(bind(listen_sock, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
  {
    perror("bind");
  }
  listen(listen_sock, BACKLOG);

  // Create a new epoll instance
  epollfd = epoll_create1(0);
  if(epollfd == -1)
  {
      perror("epoll_create1");
    return false;
  }

  // Register our listening socket with epoll
  ev.events = EPOLLIN;
  ev.data.fd = listen_sock;
  if(epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1)
  {
    perror("epoll_ctl: listen_sock");
    return false;
  }

  running = true;

  epoll_event events[MAX_EVENTS];
  int conn_sock, nfds;
  while(running)
  {
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if(nfds == -1)
    {
      perror("epoll_wait");
      running = false;
      return false;
    }

    for(int n = 0; n < nfds; ++n)
    {
      // Handle new connections
      if(events[n].data.fd == listen_sock)
      {
        struct sockaddr_storage addr;
        socklen_t addrlen = sizeof addr;
        conn_sock = accept4(listen_sock, (struct sockaddr *) &addr, &addrlen, SOCK_NONBLOCK);
        if(conn_sock == -1)
        {
          perror("accept");
          running = false;
          return false;
        }
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = conn_sock;
        if(epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1)
        {
          perror("epoll_ctl: conn_sock");
          running = false;
          return false;
        }
        handleNewConnection(conn_sock);
      }
      // Handle input from existing connection
      else
      {
        char buf[FD_BUFFER_SIZE] = "";
        int num_bytes = recv(events[n].data.fd, buf, sizeof buf, 0);
        if(num_bytes == 0)
        {
          epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, &ev);
          close(events[n].data.fd);
          handleCloseConnection(events[n].data.fd);
        }
        else if(num_bytes < 0)
        {
          perror("recv");
        }
        else
        {
          handleExistingConnection(events[n].data.fd, buf);
        }
      }
    }
  }

  close(listen_sock);
  return true;
}

int Server::sendMessage(int fd, const std::string &message)
{
  return send(fd, message.c_str(), message.length(), 0);
}
