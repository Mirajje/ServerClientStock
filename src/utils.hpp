#pragma once

#include <stddef.h>
#include <string>
#include <iostream>
#include <limits>

size_t StrToUint(const std::string& str);
void ReadUint(size_t& var, std::string_view message);