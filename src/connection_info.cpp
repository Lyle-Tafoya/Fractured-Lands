#include "connection_info.hpp"

std::unordered_map<int,ConnectionInfo *> ConnectionInfo::lookupConnectionInfoByFileDescriptor;

ConnectionInfo::ConnectionInfo(int fd) : fd(fd), player(nullptr)
{
}

int ConnectionInfo::getFileDescriptor() const
{
  return fd;
}

void ConnectionInfo::processInput(std::string const& input)
{
  inputBuffer += input;
  for(size_t newlinePosition = inputBuffer.find_first_of("\r\n"); newlinePosition != std::string::npos; newlinePosition = inputBuffer.find_first_of("\r\n"))
  {
    if(newlinePosition == 0)
    {
      inputQueue.push("");
    }
    else
    {
      inputQueue.push(inputBuffer.substr(0, newlinePosition));
    }

    for(; newlinePosition < inputBuffer.length(); ++newlinePosition)
    {
      if(inputBuffer[newlinePosition] != '\n' && inputBuffer[newlinePosition] != '\r')
      {
        inputBuffer = inputBuffer.substr(newlinePosition);
        break;
      }
      else if(newlinePosition == inputBuffer.length()-1)
      {
        inputBuffer = "";
      }
    }
  }
}
