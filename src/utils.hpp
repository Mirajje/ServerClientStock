#pragma once

#include <stddef.h>
#include <string>

size_t StrToUint(const std::string& str);
void ReadUint(size_t& var, std::string_view message);