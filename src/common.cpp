#include "common.hpp"

#include <unordered_map>

std::string ErrorCodeToMessage(ErrorCode code) {
    static std::unordered_map<ErrorCode, std::string> map = {
        {RegistrationLoginAlreadyInUse, "Login is already in use!\n"},
        {LoginInvalidLogin, "No user with this login exists!\n"},
        {LoginInvalidPassword, "Invalid password!\n"}
    };

    return map[code];
}