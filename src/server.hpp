#pragma once

#include "common.hpp"

#include <queue>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

class Session;

class Server {
public:
    Server(boost::asio::io_service& io_service);

    void HandleQuery(size_t id, size_t amount, size_t price, bool is_buying);

private:
    Balance& GetBalance(size_t id);
    void HandleAccept(Session* session, const boost::system::error_code& error);

    // id - Balance
    std::vector<Balance> balances_;

    // login - {password, id}
    std::unordered_map<std::string, std::pair<std::string, size_t>> credentials_; 

    struct CompareQueriesLess {
        bool operator()(const Query& a, const Query& b) {
            return a.price_ < b.price_ || (a.price_ == b.price_ && a.time_of_creation_ < b.time_of_creation_);
        }
    };

    struct CompareQueriesGreater {
        bool operator()(const Query& a, const Query& b) {
            return a.price_ > b.price_ || (a.price_ == b.price_ && a.time_of_creation_ < b.time_of_creation_);
        }
    };

    std::priority_queue<Query, std::vector<Query>, CompareQueriesLess> sell_queries_;
    std::priority_queue<Query, std::vector<Query>, CompareQueriesGreater> purchase_queries_;

    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;

    friend class Session;
};