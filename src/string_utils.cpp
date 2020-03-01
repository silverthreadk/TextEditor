#include "string_utils.h"

#include <regex>

bool isDigit(const std::string& str) {
    std::smatch m;
    std::regex pattern("([0-9]+)");
    return regex_match(str, m, pattern);
}
