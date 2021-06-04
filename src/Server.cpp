#include "Server.h"
#include "Service.h"

Server::Server() {}

void Server::start(asio::ip::address const& ip_address, unsigned short const port) {
    m_port = port;
    m_work = std::make_unique<asio::io_context::work>(m_ioc);
    m_ep = std::make_unique<asio::ip::tcp::endpoint>(ip_address, port);
    m_acceptor = std::make_unique<asio::ip::tcp::acceptor>(m_ioc, *m_ep);
    m_acceptor->listen();

    start_accepting();
    for (auto _{ 0 }; _ < 4; ++_) {
        auto th{ std::make_unique<std::thread>([this]() { m_ioc.run(); }) };
        m_thread_pool.push_back(std::move(th));
    }
}

void Server::start_accepting() {
    auto session{ std::make_shared<ServerSession>(m_ioc, m_port) };
    m_acceptor->async_accept(session->sock,
        [this, session](auto const& ec) {
            on_accept(session, ec);
    });
}

void Server::on_accept(std::shared_ptr<ServerSession> session, const system::error_code &ec) {
    if (!ec) {
        m_active_sessions.push_back(session);
        auto service{ std::make_shared<Service>(session, m_active_sessions, m_roomname) };

        service->startHandling();
        start_accepting();
    }
}

void Server::setRoomname(std::string_view roomname) {
    m_roomname = roomname;
}

void Server::stop() {
    m_acceptor->close();
    m_ioc.stop();
    for (auto& th : m_thread_pool)
        th->join();
}

asio::ip::address Server::address() const noexcept {
    return m_ep->address();
}
