#pragma once

#include "common.hpp"

#include <queue>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

class Session;

class Server {
public:
    Server(boost::asio::io_service& io_service);

private:
    void HandleAccept(Session* session, const boost::system::error_code& error);

    // id - Balance
    std::vector<Balance> balances_;

    class CompareQueriesLess {
        bool operator()(const Query& a, const Query& b) {
            return a.cost_ < b.cost_;
        }
    };

    class CompareQueriesGreater {
        bool operator()(const Query& a, const Query& b) {
            return a.cost_ > b.cost_;
        }
    };

    std::priority_queue<Query, std::vector<Query>, CompareQueriesLess> sell_queries_;
    std::priority_queue<Query, std::vector<Query>, CompareQueriesGreater> purchase_queries_;

    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;

    friend class Session;
};