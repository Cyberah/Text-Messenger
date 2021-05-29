#include "Server.h"
#include "Service.h"

Server::Server(const unsigned short port)
    : m_port{ port }
    , m_acceptor{ std::make_unique<asio::ip::tcp::acceptor>(m_ioc, asio::ip::tcp::endpoint{asio::ip::address_v4::any(), port}) } {
    m_acceptor->listen();
    m_work = std::make_unique<asio::io_context::work>(m_ioc);
}

void Server::start() {
    start_accepting();
    for (auto _{ 0 }; _ < 4; ++_) {
        auto th{ std::make_unique<std::thread>([this]() { m_ioc.run(); }) };
        m_thread_pool.push_back(std::move(th));
    }
    m_ioc.run();
}

void Server::start_accepting() {
    auto session{ std::make_shared<ServerSession>(m_ioc, m_port) };
    m_acceptor->async_accept(session->sock,
        [this, session](auto const& ec) {
            on_accept(session, ec);
    });
}

void Server::on_accept(std::shared_ptr<ServerSession> session, const system::error_code &ec) {
    if (ec) {
        //std::cout << "on_accept() Error occured: " << ec.message() << '\t' << ec.value() << '\n';
        return;
    }

    else {
        //std::cout << "Connection from: " << session->sock.remote_endpoint().address().to_string() << '\n';
        //m_active_sessions[session->sid] = session;
        m_active_sessions.push_back(session);

        auto service{ std::make_shared<Service>(session, m_active_sessions) };
        service->startHandling();

        start_accepting();
    }

}

void Server::stop() {
    m_acceptor->close();
    m_ioc.stop();
    for (auto& th : m_thread_pool)
        th->join();
}
