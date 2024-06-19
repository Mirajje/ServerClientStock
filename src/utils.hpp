#pragma once

#include <stddef.h>
#include <string>
#include <iostream>
#include <limits>

size_t StrToUint(const std::string& str);

template <class T>
void Cin(T& var, std::string_view message) {
    for (;;) {
        std::cout << message;
        if (std::cin >> var) {
            break;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}