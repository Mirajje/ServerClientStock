#pragma once

#include "common.hpp"
#include "json.hpp"

#include <iostream>
#include <boost/asio.hpp>
#include <memory>

class Client {
public:
    Client();

    std::string GetBalance();
    void AddQuery(size_t amount, size_t price, bool is_buying);

private:
    size_t Register();

    std::unique_ptr<boost::asio::ip::tcp::socket> socket_;
    size_t id_;
};