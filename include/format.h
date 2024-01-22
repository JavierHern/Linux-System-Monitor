#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
// Evaluates if the number is less than 10 and give fortmat
std::string timeToString(long s);
};                                    // namespace Format

#endif