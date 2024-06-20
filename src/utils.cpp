#include "utils.hpp"

#include <sstream>

size_t StrToUint(const std::string& str) {
    std::stringstream ss(str);
    size_t result;
    ss >> result;
    return result;
}

void ReadUint(size_t& var, std::string_view message) {
    for (;;) {
        std::cout << message;
        if (std::cin >> var && !(static_cast<long>(var) < 0)) {
            break;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}