#include "utils.hpp"

#include <sstream>

size_t StrToUint(const std::string& str) {
    std::stringstream ss(str);
    size_t result;
    ss >> result;
    return result;
}