#pragma once

#include <string>

inline constexpr size_t port = 5555;

struct Balance {
    size_t usd_;
    size_t rub_;
};

struct Query {
    size_t id_;
    size_t cost_;
};

enum RequestType {
    AddQuery,
    GetBalance,
    Register
};

RequestType StrToRequestType(const std::string& str);
std::string RequestTypeToStr(RequestType type);
