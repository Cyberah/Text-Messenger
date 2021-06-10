#ifndef TST_TEMP_H
#define TST_TEMP_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>
#include <QSignalSpy>

#include "../src/Client.h"
#include "../src/Server.h"

using namespace testing;

//CAUTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//AT THIS POINT SERVER(HOST) MUST BE RUNNING TO RUN PROPERLY TESTS!!!  gonna mock it later c:
//CAUTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

Q_DECLARE_METATYPE(system::error_code)
TEST(ClientTest, passingGoodArgumentsToClientConnectMethodProperlyConnects)
{
    asio::ip::address const ip_address{asio::ip::address::from_string("127.0.0.1")};
    auto const port{3333};

    Client* client = new Client();
    QSignalSpy connectedEmitted{client, SIGNAL(connected())};

    qRegisterMetaType<system::error_code>();
    QSignalSpy badConnectEmitted{client, SIGNAL(badConnect(system::error_code const&))};

    client->connect(ip_address, port, Utility::Usertype::USER);
    //std::this_thread::sleep_for(std::chrono::milliseconds(5));
    ASSERT_TRUE(connectedEmitted.wait(5000));
    EXPECT_EQ(connectedEmitted.count(), 1);
    EXPECT_EQ(badConnectEmitted.count(), 0);
}


TEST(ClientTest, passingWrongArgumentsToClientConnectMethodDoesntConnect)
{
    Client* client = new Client();
    QSignalSpy connectedEmitted{client, SIGNAL(connected())};

    //qRegisterMetaType<system::error_code>();
    QSignalSpy badConnectEmitted{client, SIGNAL(badConnect(system::error_code const&))};

    client->connect(asio::ip::address::from_string("123.4.5.6"), 9876, Utility::Usertype::USER);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    EXPECT_EQ(connectedEmitted.count(), 0);
    EXPECT_EQ(badConnectEmitted.count(), 1);
}

#endif // TST_TEMP_H
