#include <algorithm>
#include <random>
#include <fstream>

std::random_device r;
std::mt19937 random_engine(r());

bool containsBannedWord(std::string_view str)
{
  std::ifstream bannedWordFile("banned-words.txt");
  std::string word;
  std::getline(bannedWordFile, word);
  while(!bannedWordFile.eof())
  {
    if(str.find(word) != std::string::npos)
    {
      return true;
    }
    std::getline(bannedWordFile, word);
  }
  return false;
}

void splitStringOnFirst(std::string_view source, std::string &first, std::string &second, char delimeter)
{
  size_t delimeter_position = source.find_first_of(delimeter);
  if(delimeter_position == std::string::npos)
  {
    first = source;
    second = "";
  }
  else
  {
    first = source.substr(0, delimeter_position);
    second = source.substr(delimeter_position+1, source.length()-delimeter_position-1);
  }
}

void properCase(std::string &str)
{
  if(str.length() == 0) { return; }
  str[0] = toupper(str[0]);
  std::transform(str.begin()+1, str.end(), str.begin()+1, ::tolower);
}

int randomNumber(int from, int to)
{
  std::uniform_int_distribution<int> distribution(from, to);
  return distribution(random_engine);
}

int dice(int quantity, int size)
{
  int total = 0;
  for(int i = 0; i < quantity; ++i)
  {
    total += randomNumber(1, size);
  }
  return total;
}
