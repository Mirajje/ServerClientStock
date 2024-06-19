#pragma once

#include <string>

inline constexpr size_t port = 5555;

struct Balance {
    int usd_, rub_;
};

struct Query {
    size_t id_, amount_, price_;
    size_t time_of_creation_; // Is used to ensure older queries are processed sooner
};

enum RequestType {
    AddQuery,
    GetBalance,
    Register
};

RequestType StrToRequestType(const std::string& str);
std::string RequestTypeToStr(RequestType type);
