#ifndef SERVER_H
#define SERVER_H
#include "Server_Session.h"

class Server final {
public:
    Server(unsigned short const port);

    void start();

    void start_accepting();
    void on_accept(std::shared_ptr<ServerSession> session, system::error_code const& ec);

    void stop();

private:
    unsigned short const m_port;
    asio::io_context m_ioc;
    std::unique_ptr<asio::io_context::work> m_work;
    std::unique_ptr<asio::ip::tcp::acceptor> m_acceptor;
    std::vector<std::shared_ptr<ServerSession>> m_active_sessions;
    std::vector<std::unique_ptr<std::thread>> m_thread_pool;
};

#endif // SERVER_H
