#include "client.hpp"
#include "utils.hpp"

int main()
{
    try {
        Client client;

        for (;;) {
            // Тут реализовано "бесконечное" меню.
            std::cout << "Menu:\n"
                         "1) Get balance\n"
                         "2) Buy USD\n"
                         "3) Sell USD\n"
                         "4) Exit\n"
                         << std::endl;

            size_t menu_option_num;
            std::cin >> menu_option_num;
            switch (menu_option_num) {
                case 1: {
                    auto balance = client.GetBalance();
                    std::cout << "USD: " << balance.usd_ << ", " << "Rub: " << balance.rub_ << "\n";
                    break;
                }
                case 2: {
                    size_t amount;
                    size_t cost;
                    Cin(amount, "Enter USD amount: ");
                    Cin(cost, "Enter cost: ");
                    client.AddQuery(amount, cost, true);
                    std::cout << "Success!\n";
                    break;
                }
                case 3: {
                    size_t amount;
                    size_t price;
                    Cin(amount, "Enter USD amount: ");
                    Cin(price, "Enter price: ");
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