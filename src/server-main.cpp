#include "server.hpp"

#include <cstdlib>
#include <iostream>

int main() {
    try {
        boost::asio::io_service io_service;

        Server server(io_service);
        io_service.run();
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
}