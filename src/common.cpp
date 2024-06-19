#include "common.hpp"

#include <unordered_map>
#include <stdexcept>

RequestType StrToRequestType(const std::string& str) {
    static std::unordered_map<std::string, RequestType> map = {
        {"AddQuery", AddQuery}, 
        {"GetBalance", GetBalance},
        {"Register", Register}
    };

    if (!map.count(str)) {
        throw std::runtime_error("Unknown request type: " + str);
    }
    return map[str];
}

std::string RequestTypeToStr(RequestType type) {
    static std::unordered_map<RequestType, std::string> map = {
        {AddQuery, "AddQuery"},
        {GetBalance, "GetBalance"},
        {Register, "Register"}
    };

    return map[type];
}