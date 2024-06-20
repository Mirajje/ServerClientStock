#include "client.hpp"
#include "utils.hpp"

#include <memory>

namespace {

std::string ReadMessage(boost::asio::ip::tcp::socket& socket) {
    boost::asio::streambuf b;
    boost::asio::read_until(socket, b, "\0");
    std::istream is(&b);
    std::string line(std::istreambuf_iterator<char>(is), {});
    return line;
}

} // namespace

Client::Client() {
    boost::asio::io_service io_service;

    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), "127.0.0.1", std::to_string(port));
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

    socket_ = std::make_unique<boost::asio::ip::tcp::socket>(io_service);
    socket_->connect(*iterator);
}

ErrorCode Client::Register(const std::string& login, const std::string& password) {
    nlohmann::json request;
    request["ReqType"] = std::to_string(RequestType::Register);
    request["Login"] = login;
    request["Password"] = password;

    boost::asio::write(*socket_, boost::asio::buffer(request.dump()));
    auto answer = nlohmann::json::parse(ReadMessage(*socket_));
    if (answer.contains("ErrorCode")) {
        return ErrorCode(std::stoi(static_cast<std::string>(answer["ErrorCode"])));
    }
    id_ = StrToUint(static_cast<std::string>(answer["Id"]));
    return NoErrors;
}

ErrorCode Client::Login(const std::string& login, const std::string& password) {
    nlohmann::json request;
    request["ReqType"] = std::to_string(RequestType::Login);
    request["Login"] = login;
    request["Password"] = password;

    boost::asio::write(*socket_, boost::asio::buffer(request.dump()));
    auto answer = nlohmann::json::parse(ReadMessage(*socket_));
    if (answer.contains("ErrorCode")) {
        return ErrorCode(std::stoi(static_cast<std::string>(answer["ErrorCode"])));
    }
    id_ = StrToUint(static_cast<std::string>(answer["Id"]));
    return NoErrors;
}

Balance Client::GetBalance() {
    if (!id_) {
        throw std::runtime_error("Client must register or log in before any actions can be taken\n");
    }

    nlohmann::json request;
    request["Id"] = std::to_string(*id_);
    request["ReqType"] = std::to_string(RequestType::GetBalance);


    boost::asio::write(*socket_, boost::asio::buffer(request.dump()));
    auto answer = nlohmann::json::parse(ReadMessage(*socket_));
    return {std::stoi(static_cast<std::string>(answer["USD"])), std::stoi(static_cast<std::string>(answer["Rub"]))};
}

void Client::AddQuery(size_t amount, size_t price, bool is_buying) {
    if (!id_) {
        throw std::runtime_error("Client must register or log in before any actions can be taken\n");
    }

    nlohmann::json request;
    request["Id"] = std::to_string(*id_);
    request["ReqType"] = std::to_string(RequestType::AddQuery);
    request["Amount"] = std::to_string(amount);
    request["Price"] = std::to_string(price);
    request["IsBuying"] = std::to_string(is_buying);

    boost::asio::write(*socket_, boost::asio::buffer(request.dump()));
    ReadMessage(*socket_);
}