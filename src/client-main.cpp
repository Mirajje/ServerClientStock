#include "client.hpp"
#include "utils.hpp"

#include <iostream>

int main() {
    try {
        Client client;

        ErrorCode error_code = RegistrationLoginAlreadyInUse; // Doesn't matter which, just not 0
        while (error_code != ErrorCode::NoErrors) {
            size_t auth_menu_option_num;
            ReadUint(auth_menu_option_num, "Menu:\n"
                                      "1) Login\n"
                                      "2) Register\n"
                                      "Enter a command: ");

            std::string login, password;
            std::cout << "Enter login: ";
            std::cin >> login;
            std::cout << "Enter password: ";
            std::cin >> password;

            switch (auth_menu_option_num) {
                case 1: {
                    error_code = client.Login(login, password);
                    break;
                }
                case 2: {
                    error_code = client.Register(login, password);
                    break;
                }
            }

            if (error_code == ErrorCode::NoErrors) {
                break;
            }

            std::cout << ErrorCodeToMessage(error_code) << "\n";
        }
        std::cout << "Logged in succesfully!\n";

        for (;;) {
            size_t menu_option_num;
            ReadUint(menu_option_num, "\nMenu:\n"
                         "1) Get balance\n"
                         "2) Buy USD\n"
                         "3) Sell USD\n"
                         "4) Exit\n"
                         "Enter a command: ");

            switch (menu_option_num) {
                case 1: {
                    auto balance = client.GetBalance();
                    std::cout << "USD: " << balance.usd_ << ", " << "Rub: " << balance.rub_ << "\n";
                    break;
                }
                case 2: {
                    size_t amount;
                    size_t cost;
                    ReadUint(amount, "Enter USD amount: ");  
                    ReadUint(cost, "Enter cost: ");
                    client.AddQuery(amount, cost, true);
                    std::cout << "Success!\n";
                    break;
                }
                case 3: {
                    size_t amount;
                    size_t price;
                    ReadUint(amount, "Enter USD amount: ");
                    ReadUint(price, "Enter price: ");
                    client.AddQuery(amount, price, false);
                    std::cout << "Success!\n";
                    break;
                }
                case 4: {
                    return 0;
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
}