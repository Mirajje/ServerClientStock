#pragma once

#include "common.hpp"
#include "json.hpp"

#include <boost/asio.hpp>

#include <iostream>
#include <memory>
#include <optional>

class Client {
public:
    Client();
    ErrorCode Login(const std::string& login, const std::string& password);
    ErrorCode Register(const std::string& login, const std::string& password);

    Balance GetBalance();
    void AddQuery(size_t amount, size_t price, bool is_buying);

private:
    std::unique_ptr<boost::asio::ip::tcp::socket> socket_;
    std::optional<size_t> id_;
};