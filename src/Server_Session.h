#ifndef SERVER_SESSION_H
#define SERVER_SESSION_H
#include <boost/asio.hpp>
using namespace boost;

struct ServerSession final {
    ServerSession(asio::io_context& ioc, unsigned short const port)
        : sock{ ioc }
        , ep { asio::ip::address_v4::any(), port } {}

    asio::ip::tcp::socket sock;
    asio::ip::tcp::endpoint ep;
    std::string user_name;
};

#endif // SERVER_SESSION_H
