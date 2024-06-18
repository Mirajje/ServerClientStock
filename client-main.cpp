#include "client.hpp"

int main()
{
    try {
        Client client;

        for (;;) {
            // Тут реализовано "бесконечное" меню.
            std::cout << "Menu:\n"
                         "1) Get balance\n"
                         "2) Exit\n"
                         << std::endl;

            size_t menu_option_num;
            std::cin >> menu_option_num;
            switch (menu_option_num) {
                case 1: {
                    std::cout << client.GetBalance() << "\n";
                    break;
                }
                case 2: {
                    exit(0);
                    break;
                }
                default: {
                    std::cout << "Unknown menu option\n" << std::endl;
                }
            }
        }
    }
    catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}