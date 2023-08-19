#ifndef FRACTURED_LANDS_UTILITIES_HPP_
#define FRACTURED_LANDS_UTILITIES_HPP_

#include <string>
#include <string_view>

extern bool containsBannedWord(std::string_view str);
extern void splitStringOnFirst(std::string_view source, std::string &first, std::string &second, char delimeter=' ');
extern void properCase(std::string &str);
extern int randomNumber(int from, int to);
extern int dice(int quantity, int size);

#endif
