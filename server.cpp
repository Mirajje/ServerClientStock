#include "server.hpp"
#include "json.hpp"
#include "utils.hpp"

#include <iostream>

namespace {
    constexpr size_t max_size = 1024;
}

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

        std::string reply;
        switch (reqType) {
            case RequestType::Register: {
                reply = std::to_string(server_.balances_.size());
                server_.balances_.emplace_back();
                break;
            }
            case RequestType::AddQuery: {
                return;
            }
            case RequestType::GetBalance: {
                size_t id = StrToUint(json["Id"]);

                if (id >= server_.balances_.size()) {
                    throw std::runtime_error("Invalid client id: " + std::to_string(id));
                }

                auto balance = server_.balances_[id];
                reply = "RUB: " + std::to_string(balance.rub_) + ", USD: " + std::to_string(balance.usd_);
                break;
            }
            default: {
                return;
            }
        }

        boost::asio::async_write(socket_,
            boost::asio::buffer(reply, reply.size()),
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

void Server::HandleAccept(Session* session,
        const boost::system::error_code& error) {
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