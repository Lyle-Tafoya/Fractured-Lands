#include "colors.hpp"

const std::unordered_map<char,std::string_view> COLOR_MAP =
{
  { 'n', COLOR_NORMAL },
  { 'k', COLOR_BLACK },
  { 'K', COLOR_BRIGHT_BLACK },
  { 'r', COLOR_RED },
  { 'R', COLOR_BRIGHT_RED },
  { 'g', COLOR_GREEN },
  { 'G', COLOR_BRIGHT_GREEN },
  { 'y', COLOR_YELLOW },
  { 'Y', COLOR_BRIGHT_YELLOW },
  { 'b', COLOR_BLUE },
  { 'B', COLOR_BRIGHT_BLUE },
  { 'm', COLOR_MAGENTA },
  { 'M', COLOR_BRIGHT_MAGENTA },
  { 'c', COLOR_FAINT_CYAN },
  { 'C', COLOR_BRIGHT_CYAN },
  { 'w', COLOR_WHITE },
  { 'W', COLOR_BRIGHT_WHITE }
};

void colorizeText(std::string &str)
{
  for(size_t i=0; i < str.length(); ++i)
  {
    if(str[i] == '|' && i < (str.length()-1))
    {
      auto colorCode = COLOR_MAP.find(str[i+1]);
      if(colorCode == COLOR_MAP.end())
      {
        ++i;
        continue;
      }
      str.erase(i, 2);
      str.insert(i, colorCode->second);
    }
  }
}

