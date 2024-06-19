#include "server.hpp"
#include "json.hpp"
#include "utils.hpp"

#include <iostream>
#include <sstream>

namespace {

constexpr size_t max_size = 1024;

void SellUSD(Balance& from, Balance& to, size_t amount, size_t price) {
    from.rub_ += amount* price;
    to.rub_ -= amount* price;
    from.usd_ -= amount;
    to.usd_ += amount;
}

} // namespace

class Session {
public:
    Session(boost::asio::io_service& io_service, Server& server);

    void Start();
    boost::asio::ip::tcp::socket& GetSocket();

private:
    void HandleRead(const boost::system::error_code& error, size_t bytes_transferred);
    void HandleWrite(const boost::system::error_code& error);

    boost::asio::ip::tcp::socket socket_;
    Server& server_;
    char data_[max_size];
};

Session::Session(boost::asio::io_service& io_service, Server& server)
    : socket_(io_service), server_(server) {}

void Session::Start() {
    socket_.async_read_some(boost::asio::buffer(data_, max_size),
        boost::bind(&Session::HandleRead, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

boost::asio::ip::tcp::socket& Session::GetSocket() {
    return socket_;
}

void Session::HandleRead(const boost::system::error_code& error,
        size_t bytes_transferred) {
    try {
        if (error) {
            delete this;
            return;
        }

        data_[bytes_transferred] = '\0';

        auto json = nlohmann::json::parse(data_);
        auto reqType = StrToRequestType(json["ReqType"]);

        nlohmann::json reply;
        switch (reqType) {
            case RequestType::Register: {
                reply["Id"] = std::to_string(server_.balances_.size());
                server_.balances_.emplace_back();
                break;
            }
            case RequestType::AddQuery: {
                size_t id = StrToUint(json["Id"]);

                if (id >= server_.balances_.size()) {
                    throw std::runtime_error("Invalid client id: " + std::to_string(id));
                }

                size_t amount = StrToUint(json["Amount"]);
                size_t price = StrToUint(json["Price"]);
                bool is_buying;
                std::istringstream(static_cast<std::string>(json["IsBuying"])) >> is_buying;
                server_.HandleQuery(id, amount, price, is_buying);
                break;
            }
            case RequestType::GetBalance: {
                size_t id = StrToUint(json["Id"]);

                if (id >= server_.balances_.size()) {
                    throw std::runtime_error("Invalid client id: " + std::to_string(id));
                }

                auto& balance = server_.GetBalance(id);
                reply["Rub"] = std::to_string(balance.rub_);
                reply["USD"] = std::to_string(balance.usd_);
                break;
            }
        }

        std::string dump = reply.dump();
        boost::asio::async_write(socket_,
            boost::asio::buffer(dump, dump.size()),
            boost::bind(&Session::HandleWrite, this,
                boost::asio::placeholders::error));

    } catch (const std::runtime_error& err) {
        std::cout << err.what() << "\n";
        delete this;
    }
}

void Session::HandleWrite(const boost::system::error_code& error) {
    if (error) {
        delete this;
        return;
    }

    socket_.async_read_some(boost::asio::buffer(data_, max_size),
        boost::bind(&Session::HandleRead, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

Server::Server(boost::asio::io_service& io_service)
        : io_service_(io_service), acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
    std::cout << "Server started! Listen " << port << " port" << std::endl;

    Session* new_session = new Session(io_service_, *this);
    acceptor_.async_accept(new_session->GetSocket(),
        boost::bind(&Server::HandleAccept, this, new_session,
            boost::asio::placeholders::error));
}

void Server::HandleAccept(Session* session, const boost::system::error_code& error) {
    if (error) {
        delete session;
        return;
    }

    session->Start();
    auto new_session = new Session(io_service_, *this);
    acceptor_.async_accept(new_session->GetSocket(),
        boost::bind(&Server::HandleAccept, this, new_session,
            boost::asio::placeholders::error));
}

Balance& Server::GetBalance(size_t id) {
    if (id >= balances_.size()) {
        throw std::runtime_error("Invalid client id: " + std::to_string(id));
    }

    return balances_[id];
}

void Server::HandleQuery(size_t id, size_t amount, size_t price, bool is_buying) {
    static std::atomic<size_t> query_priority{};

    auto& balance = GetBalance(id);

    if (is_buying) {
        while (!sell_queries_.empty() && sell_queries_.top().price_ <= price) {
            auto& best_query = sell_queries_.top();
            auto& seller_balance = GetBalance(best_query.id_);

            size_t exchange_amount = std::min(best_query.amount_, amount);
            SellUSD(seller_balance, balance, exchange_amount, best_query.price_);
            amount -= exchange_amount;

            if (best_query.amount_ == exchange_amount) {
                sell_queries_.pop();
            }

            if (!amount) {
                break;
            }
        }

        if (amount) {
            purchase_queries_.push({id, amount, price, query_priority++});
        }

        return;
    }

    while (!purchase_queries_.empty() && purchase_queries_.top().price_ >= price) {
        auto& best_query = purchase_queries_.top();
        auto& buyer_balance = GetBalance(best_query.id_);

        size_t exchange_amount = std::min(best_query.amount_, amount);
        SellUSD(balance, buyer_balance, exchange_amount, best_query.price_);
        amount -= exchange_amount;

        if (best_query.amount_ == exchange_amount) {
            purchase_queries_.pop();
        }

        if (!amount) {
            break;
        }
    }

    if (amount) {
        sell_queries_.push({id, amount, price, query_priority++});
    }
}