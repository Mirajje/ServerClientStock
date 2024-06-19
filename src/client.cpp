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

    id_ = Register();
}

size_t Client::Register() {
    nlohmann::json request;
    request["ReqType"] = RequestTypeToStr(RequestType::Register);

    boost::asio::write(*socket_, boost::asio::buffer(request.dump()));
    auto answer = nlohmann::json::parse(ReadMessage(*socket_));
    return StrToUint(static_cast<std::string>(answer["Id"]));
}

Balance Client::GetBalance() {
    nlohmann::json request;
    request["Id"] = std::to_string(id_);
    request["ReqType"] = RequestTypeToStr(RequestType::GetBalance);


    boost::asio::write(*socket_, boost::asio::buffer(request.dump()));
    auto answer = nlohmann::json::parse(ReadMessage(*socket_));
    return {std::stoi(static_cast<std::string>(answer["USD"])), std::stoi(static_cast<std::string>(answer["Rub"]))};
}

void Client::AddQuery(size_t amount, size_t price, bool is_buying) {
    nlohmann::json request;
    request["Id"] = std::to_string(id_);
    request["ReqType"] = RequestTypeToStr(RequestType::AddQuery);
    request["Amount"] = std::to_string(amount);
    request["Price"] = std::to_string(price);
    request["IsBuying"] = std::to_string(is_buying);

    boost::asio::write(*socket_, boost::asio::buffer(request.dump()));
    ReadMessage(*socket_);
}