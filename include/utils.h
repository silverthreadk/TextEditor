#ifndef UTILS_H_
#define UTILS_H_

#include <string>

void clearScreen();

std::pair<int, int> getScreenSize();

int getConsoleCursor();

bool isDigit(const std::string& str);

#endif  // UTILS_H_
