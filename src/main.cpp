#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "mongo_controller.hpp"
#include "player.hpp"
#include "server.hpp"
#include "world.hpp"

std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("basic_logger", "game.log");
DatabaseController database_controller;
Server server;
World world(
  {360,360,360,4,268,39,170.f,0.26f,2.7f,-4.59999f,217,250,269},
  {{45,23},{120,60}}
);

int main(int argc, char **argv)
{
  spdlog::set_level(spdlog::level::debug);
  logger->flush_on(spdlog::level::info);
  unsigned int port = (argc > 1 ? std::stoi(argv[1]) : 5555);

  logger->info("Starting server on port {}", port);
  server.registerNewConnectionCallback(Player::handleNewPlayerConnection);
  server.registerExistingConnectionCallback(Player::handleExistingPlayerConnection);
  server.registerCloseConnectionCallback(Player::handleClosePlayerConnection);

  server.start(port);
  return 0;
}
