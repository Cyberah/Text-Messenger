#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H
#include <boost/asio.hpp>

using namespace boost;

struct ClientSession final {
    ClientSession(asio::io_context& ioc, asio::ip::address const& ip_address, unsigned short const port)
        : ep{ asio::ip::tcp::endpoint{ip_address, port} }
        , sock{ ioc } {}

    asio::ip::tcp::endpoint ep;
    asio::ip::tcp::socket sock;
};

#endif // CLIENT_SESSION_H
