#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <spdlog/spdlog.h>
#include <string_view>
#include <fmt/core.h>

#include "commands.hpp"
#include "colors.hpp"
#include "mongo_controller.hpp"
#include "player.hpp"
#include "server.hpp"
#include "sha256.hpp"
#include "telnet.hpp"
#include "utilities.hpp"
#include "world.hpp"

extern std::shared_ptr<spdlog::logger> logger;
extern Server server;
extern World world;
extern DatabaseController database_controller;

std::string Player::playerWelcomeText;
std::unordered_map<int, Player *> Player::lookupPlayerByFileDescriptor;
std::unordered_map<std::string, Player *> Player::lookupPlayerByName;

const std::unordered_map<std::string, std::string> Player::prompt_templates =
{
  {"cronus", "|K[|nHP=%c|n/|G%H |nSP=%C|n/|G%S|K]"},
  {"oceanus", "|c[|CHealth: %p%|c]"},
  {"rhea", "|r[|R%h|r/|R%H|rhp |R%s|r/|R%S|rsp]"},
  {"atlas","|G<< |Y%h/%Hhp |C%s/%Ssp |G>>"},
  {"themis","%c|n/%C"},
  {"off",""}
};

const Creature *Player::getBody() const
{
  return body;
}

const std::string &Player::getPrompt() const
{
  return prompt;
}

const std::unordered_map<int, Player *> &Player::getPlayerFDMap()
{
  return lookupPlayerByFileDescriptor;
}

const std::unordered_map<std::string, Player *> &Player::getPlayerNameMap()
{
  return lookupPlayerByName;
}

Player::Player(const std::string &name, ConnectionInfo *connection)
  : connection(connection), name(name)
{
  lookupPlayerByFileDescriptor[connection->getFileDescriptor()] = this;
  lookupPlayerByName[name] = this;
}
Player::~Player()
{
  if(primaryBody)
  {
    delete primaryBody;
  }
}

void Player::receiveMessage(const std::string &message) const
{
  std::string result;
  for(size_t i = 0; i < message.length(); ++i)
  {
    if(message[i] == '|' && i < (message.length()-1))
    {
      if(color_enabled)
      {
        auto color_code = COLOR_MAP.find(message[i+1]);
        if(color_code != COLOR_MAP.end())
        {
          result += color_code->second;
        }
      }
      ++i;
    }
    else
    {
      result += message[i];
    }
  }

  Server::sendMessage(connection->fd, result);
}

bool Player::getColorEnabled() const
{
  return color_enabled;
}

const std::string &Player::getName() const
{
  return name;
}

int Player::getFileDescriptor() const
{
  return connection->getFileDescriptor();
}

const std::string &Player::getPassword() const
{
  return password;
}

bool Player::getVerbose() const
{
  return verbose;
}

void Player::setColorEnabled(bool enabled)
{
  color_enabled = enabled;
}

void Player::showPrompt() const
{
  std::string result;
  for(size_t i = 0; i < prompt.length(); ++i)
  {
    if(prompt[i] == '%' && i < (prompt.length()-1))
    {
      switch(prompt[i+1])
      {
        case 'h':
          result += std::to_string(body->getHp());
          break;
        case 'H':
          result += std::to_string(body->getMaxHp());
          break;
        case 's':
          result += std::to_string(body->getSp());
          break;
        case 'S':
          result += std::to_string(body->getMaxSp());
          break;
        case 'p':
          result += std::to_string(body->getHp()*100/body->getMaxHp());
          break;
        case 'P':
          result += std::to_string(body->getSp()*100/body->getMaxSp());
          break;
        case 'c':
          {
            int hp_percent = body->getHp()*100/body->getMaxHp();
            if(hp_percent < 25)
            {
              result += "|R" + std::to_string(body->getHp()) + "|n";
            }
            else if(hp_percent < 50)
            {
              result += "|Y" + std::to_string(body->getHp()) + "|n";
            }
            else
            {
              result += "|G" + std::to_string(body->getHp()) + "|n";
            }
          }
          break;
        case 'C':
          {
            int sp_percent = body->getSp()*100/body->getMaxSp();
            if(sp_percent < 25)
            {
              result += "|R" + std::to_string(body->getSp()) + "|n";
            }
            else if(sp_percent < 50)
            {
              result += "|Y" + std::to_string(body->getSp()) + "|n";
            }
            else
            {
              result += "|G" + std::to_string(body->getSp()) + "|n";
            }
          }
          break;
        default:
          result += "%";
          --i;
      }
      ++i;
    }
    else
    {
      result.push_back(prompt[i]);
    }
  }
  result += "|n ";
  receiveMessage(result);
}

void Player::setPassword(std::string_view password)
{
  this->password = password;
}

void Player::setVerbose(bool verbose)
{
  this->verbose = verbose;
}

void Player::setPrompt(std::string_view prompt)
{
  this->prompt = prompt;
}

void Player::handleColorCheckInput(Player &player)
{
  std::string &input = player.connection->inputQueue.front();
  if(!input.empty() && (input[0] == 'y' || input[0] == 'Y'))
  {
    player.setColorEnabled(true);
  }
  else
  {
    player.setColorEnabled(false);
  }
  player.connection->inputQueue.pop();

  player.state = PLAYING;
  player.body->soul = &player;
  player.body->setRoom(world.flattenCoordinates({54, 98, world.terrain[54][98].height, 0}));
  player.body->setName(player.name);
  player.body->setChutzpah(6+randomNumber(0, 2));
  player.body->setConstitution(6+randomNumber(0, 2));
  player.body->setDexterity(6+randomNumber(0, 2));
  player.body->setIntelligence(6+randomNumber(0, 2));
  player.body->setPower(6+randomNumber(0, 2));
  player.body->setSanity(6+randomNumber(0, 2));
  player.body->setSize(6+randomNumber(0, 2));
  player.body->setStrength(6+randomNumber(0, 2));
  player.body->setWisdom(6+randomNumber(0, 2));
  player.body->setHp(player.body->getMaxHp());
  player.body->setSp(player.body->getMaxSp());

  auto result = database_controller.createCreature(*player.body);
  if(result)
  {
    player.body->id = result.value();
  }
  database_controller.createPlayer(player);

  player.body->broadcastToRoom(fmt::format("{} enters the game.\r\n", player.name));
  player.body->doCommand("look", "");
  player.showPrompt();
}

void Player::handleGameInput(Player &player)
{
  std::string &input = player.connection->inputQueue.front();
  int fd = player.getFileDescriptor();
  std::string command;
  std::string argument;
  if(input.size() > 0)
  {
    if((input[0] >= 'a' && input[0] <= 'z') || (input[0] >= 'A' && input[0] <= 'Z'))
    {
      splitStringOnFirst(input, command, argument);
    }
    else
    {
      command = input[0];
      argument = input.substr(1, input.length()-1);
    }

    player.body->doCommand(command, argument);
  }
  if(lookupPlayerByFileDescriptor.contains(fd))
  {
    player.showPrompt();
    player.connection->inputQueue.pop();
    if(!player.connection->inputQueue.empty())
    {
      handleGameInput(player);
    }
  }
}

void Player::handleNewPasswordInput(Player &player)
{
  std::string &input = player.connection->inputQueue.front();

  if(input.length() == 0)
  {
    player.receiveMessage("\r\nTry again another time then.\r\n");
    server.closeConnection(player.getFileDescriptor());
    return;
  }
  if(input.length() < 6)
  {
    player.receiveMessage("\r\nThe password must be at least 6 characters long.\r\nPassword: ");
    player.connection->inputQueue = std::queue<std::string>();
    return;
  }

  if(player.getPassword().empty())
  {
    player.setPassword(sha256(input));
    player.receiveMessage("\r\nPassword: (again) ");
    player.connection->inputQueue.pop();
    return;
  }
  if(player.getPassword() != sha256(input))
  {
    player.receiveMessage("\r\nYour passwords didn't match.\r\n");
    server.closeConnection(player.getFileDescriptor());
    return;
  }
  player.receiveMessage({ IAC, WONT, ECHO, static_cast<char>(0) });
  player.receiveMessage("\r\n");

  std::string colorPrompt = "Colour check: are these words |Ggreen|n and |Yyellow|n?\r\n[Enter \"y\" or \"n\"]: ";
  colorizeText(colorPrompt);
  player.receiveMessage(colorPrompt);
  player.state = COLOR_CHECK;

  player.connection->inputQueue.pop();
}

void Player::handleExistingPasswordInput(Player &player)
{
  std::string &input = player.connection->inputQueue.front();
  if(player.getPassword() != sha256(input))
  {
    player.receiveMessage("\r\nWrong password.\r\n");
    server.closeConnection(player.getFileDescriptor());
    return;
  }
  player.receiveMessage({ IAC, WONT, ECHO, static_cast<char>(0) });
  player.receiveMessage("\r\n");
  logger->info("Player {} entering the world", player.name);

  player.body->soul = &player;
  player.state = PLAYING;

  player.connection->inputQueue.pop();

  player.body->broadcastToRoom(fmt::format("{} enters the game.\r\n", player.name));
  player.body->doCommand("look", "");
  player.showPrompt();
}

bool Player::load()
{
  body = new Creature();
  return database_controller.readPlayer(name, *this, *body);
}
bool Player::save()
{
  return database_controller.updatePlayer(*this);
}

void Player::handleUsernameInput(ConnectionInfo &connection)
{
  std::string &input = connection.inputQueue.front();
  bool invalidName = false;
  if(input.length() == 0)
  {
    Server::sendMessage(connection.getFileDescriptor(), "Come back another time then!\r\n");
    server.closeConnection(connection.getFileDescriptor());
    return;
  }
  else if(input.length() < 3)
  {
    Server::sendMessage(connection.getFileDescriptor(), "Name too short.\r\n");
    invalidName = true;
  }
  else if(input.length() > 20)
  {
    Server::sendMessage(connection.getFileDescriptor(), "Name too long.\r\n");
    invalidName = true;
  }
  else if(containsBannedWord(input))
  {
    Server::sendMessage(connection.getFileDescriptor(), "That name is reserved for a former hero.\r\n");
    invalidName = true;
  }
  else
  {
    // Check for invalid characters
    for(size_t i = 0; i < input.length(); ++i)
    {
      if(!((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z')))
      {
        Server::sendMessage(connection.getFileDescriptor(), "Invalid characters in name:" + input +
            "\r\nCharacter number was " + std::to_string(i) + "\r\n");
        invalidName = true;
        break;
      }
    }
  }
  if(invalidName)
  {
    Server::sendMessage(connection.getFileDescriptor(), "Give name again: ");
    connection.inputQueue = std::queue<std::string>();
    return;
  }
  properCase(input);
  connection.player = new Player(input, &connection);
  connection.inputQueue.pop();
  if(connection.player->load())
  {
    Server::sendMessage(connection.getFileDescriptor(), "Password: ");
    Server::sendMessage(connection.getFileDescriptor(), { IAC, WILL, ECHO, static_cast<char>(0) });
    connection.player->state = EXISTING_PASSWORD;
    if(!connection.inputQueue.empty())
    {
      handleExistingPasswordInput(*connection.player);
    }
  }
  else
  {
    Server::sendMessage(connection.getFileDescriptor(), "Pick a password that is difficult to guess (at least 6 letters).\r\nPassword:");
    Server::sendMessage(connection.getFileDescriptor(), { IAC, WILL, ECHO, static_cast<char>(0) });
    connection.player->state = NEW_PASSWORD;
    if(!connection.inputQueue.empty())
    {
      handleNewPasswordInput(*connection.player);
    }
  }
}

void Player::handleClosePlayerConnection(int fd)
{
  Player *player = lookupPlayerByFileDescriptor[fd];
  lookupPlayerByFileDescriptor.erase(fd);
  lookupPlayerByName.erase(player->getName());
  delete player;
}

void Player::handleExistingPlayerConnection(int fd, std::string input)
{
  // Don't bother processing IAC responses
  if(input[0] == IAC)
  {
    return;
  }

  ConnectionInfo *connection = ConnectionInfo::lookupConnectionInfoByFileDescriptor[fd];
  connection->processInput(input);
  if(connection->inputQueue.empty())
  {
    return;
  }
  if(connection->player == nullptr)
  {
    handleUsernameInput(*connection);
    return;
  }

  switch(connection->player->state)
  {
    case NEW_PASSWORD:
      handleNewPasswordInput(*connection->player);
      break;
    case EXISTING_PASSWORD:
      handleExistingPasswordInput(*connection->player);
      break;
    case COLOR_CHECK:
      handleColorCheckInput(*connection->player);
      break;
    case PLAYING:
      handleGameInput(*connection->player);
      break;
    default:
      break;
  }
}

void Player::handleNewPlayerConnection(int fd)
{
  if(playerWelcomeText.empty())
  {
    std::ifstream f("welcome.txt");
    std::stringstream buffer;
    buffer << f.rdbuf();
    playerWelcomeText = buffer.str();
  }
  std::time_t systemTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::string message = "\r\n" + playerWelcomeText + "\r\n\r\n"
    "Welcome to Fractured Lands\r\n" +
    "The local time is " + std::ctime(&systemTime) + "\r\n" +
    "New players: choose a name for your character, or enter 'guest'.\r\n\r\n" +
    "What is your name: ";

  Server::sendMessage(fd, message);

  ConnectionInfo *connection = new ConnectionInfo(fd);
  ConnectionInfo::lookupConnectionInfoByFileDescriptor[fd] = connection;
}
