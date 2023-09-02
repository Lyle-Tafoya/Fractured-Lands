/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
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
