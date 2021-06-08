#ifndef SERVER_H
#define SERVER_H
#include <QObject>
#include "Server_Session.h"
#include "Service.h"

class Server {
public:
                      Server();

    void              start(asio::ip::address const& ip_address, unsigned short const port);
    void              start_accepting();

    void              on_accept(std::shared_ptr<ServerSession> session, system::error_code const& ec);
    void              setRoomname(std::string_view roomname);

    void              stop();
    asio::ip::address address() const noexcept;

private:
    unsigned short m_port;
    asio::io_context m_ioc;
    std::string m_roomname;
    std::unique_ptr<asio::ip::tcp::endpoint> m_ep;
    std::unique_ptr<asio::io_context::work> m_work;
    std::unique_ptr<asio::ip::tcp::acceptor> m_acceptor;
    std::vector<std::shared_ptr<ServerSession>> m_active_sessions;
    std::vector<std::unique_ptr<std::thread>> m_thread_pool;
};

#endif // SERVER_H
