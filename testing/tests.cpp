#include "../src/server.hpp"
#include "../src/client.hpp"

#include <gtest/gtest.h>
#include <boost/thread.hpp>

class Testsuite {
public:
    Testsuite() 
        : server_(io_service_), t_(boost::bind(&boost::asio::io_service::run, &io_service_)) {}

    ~Testsuite() {
        io_service_.stop();
        t_.join();
    }

    boost::asio::io_service io_service_;
    Server server_;
    std::thread t_;
};

TEST(StockTests, PartialCompletion) {
    Testsuite ts;

    Client client1;
    Client client2;

    client1.AddQuery(10, 10, true);
    client2.AddQuery(15, 9, false);

    ASSERT_EQ(client1.GetBalance().rub_, -100);
    ASSERT_EQ(client1.GetBalance().usd_, 10);
    ASSERT_EQ(client2.GetBalance().rub_, 100);
    ASSERT_EQ(client2.GetBalance().usd_, -10);

    client1.AddQuery(5, 15, true);

    ASSERT_EQ(client1.GetBalance().rub_, -145);
    ASSERT_EQ(client1.GetBalance().usd_, 15);
    ASSERT_EQ(client2.GetBalance().rub_, 145);
    ASSERT_EQ(client2.GetBalance().usd_, -15);
}

TEST(StockTests, MultipleQueries) {
    Testsuite ts;

    Client client1;
    Client client2;
    Client client3;

    client1.AddQuery(10, 10, true);
    client2.AddQuery(15, 9, true);
    client3.AddQuery(25, 9, false);

    ASSERT_EQ(client1.GetBalance().rub_, -100);
    ASSERT_EQ(client1.GetBalance().usd_, 10);
    ASSERT_EQ(client2.GetBalance().rub_, -135);
    ASSERT_EQ(client2.GetBalance().usd_, 15);
    ASSERT_EQ(client3.GetBalance().rub_, 235);
    ASSERT_EQ(client3.GetBalance().usd_, -25);
}