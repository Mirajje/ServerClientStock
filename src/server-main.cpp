#include "server.hpp"

#include <cstdlib>
#include <iostream>

int main() {
    try {
        boost::asio::io_service io_service;

        Server server(io_service);
        io_service.run();
    } catch (const std::exception& exc) {
        std::cout << "Exception: " << exc.what() << "\n";
    }

    return 0;
}