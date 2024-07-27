#ifndef _COLORS_HPP
#define _COLORS_HPP

#include <iostream>
#include <sstream>
#include <string>

/* FOREGROUND */
#define RST "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#define FRED(x) (std::string(KRED) + toString(x) + RST)
#define FGRN(x) (std::string(KGRN) + toString(x) + RST)
#define FYEL(x) (std::string(KYEL) + toString(x) + RST)
#define FBLU(x) (std::string(KBLU) + toString(x) + RST)
#define FMAG(x) (std::string(KMAG) + toString(x) + RST)
#define FCYN(x) (std::string(KCYN) + toString(x) + RST)
#define FWHT(x) (std::string(KWHT) + toString(x) + RST)

/* BACKGROUND */
#define KRED40 "\x1B[41m"
#define KGRN40 "\x1B[42m"
#define KYEL40 "\x1B[43m"
#define KBLU40 "\x1B[44m"
#define KMAG40 "\x1B[45m"
#define KCYN40 "\x1B[46m"
#define KWHT40 "\x1B[47m"

#define BRED(x) (std::string(KRED40) + toString(x) + RST)
#define BGRN(x) (std::string(KGRN40) + toString(x) + RST)
#define BYEL(x) (std::string(KYEL40) + toString(x) + RST)
#define BBLU(x) (std::string(KBLU40) + toString(x) + RST)
#define BMAG(x) (std::string(KMAG40) + toString(x) + RST)
#define BCYN(x) (std::string(KCYN40) + toString(x) + RST)
#define BWHT(x) (std::string(KWHT40) + toString(x) + RST)

/* STYLES */
#define BOLD(x) (std::string("\x1B[1m") + toString(x) + "\x1B[0m")
#define ITAL(x) (std::string("\x1B[3m") + toString(x) + "\x1B[0m")
#define UNDL(x) (std::string("\x1B[4m") + toString(x) + "\x1B[0m")

template <typename T>
std::string toString(const T &value)
{
  std::stringstream ss;
  ss << value;
  return ss.str();
}

#endif /* _COLORS_HPP */
