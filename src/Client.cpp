#include "Client.h"
#include "Client_Session.h"
#include "Utility.h"

Client::Client() {
    m_work = std::make_unique<asio::io_context::work>(m_ioc);
}

void Client::connect(asio::ip::address const& ip_address, const unsigned short port) {
    m_session = std::make_shared<ClientSession>(m_ioc, ip_address, port);
    m_session->sock.async_connect(m_session->ep,
        [this](auto const& ec) {
            on_connected(ec);
    });

    for (auto i{ 0 }; i < 4; ++i) {
        auto th{ std::make_unique<std::thread>([this]() { m_ioc.run(); }) };
        m_thread_pool.push_back(std::move(th));
    }
}

void Client::disconnect() {
    m_session->sock.shutdown(asio::ip::tcp::socket::shutdown_both);
    m_ioc.stop();
    m_readThread->join();
    for (auto& th : m_thread_pool)
        th->join();
}

void Client::on_connected(const system::error_code &ec) {
    if (!ec) {
        emit connected();
        write_username();
        handle_read();
    }
    else
        emit badConnect(ec);
}

void Client::write_username() {
    auto const user_name_complete{m_user_name + '\n'};
    asio::async_write(m_session->sock, asio::buffer(&user_name_complete[0], user_name_complete.length()),
        [this](auto const& ec, auto const /*bytes_transferred*/) {
            if (ec)
                emit errorOccured(ec);
    });
}

void Client::write(std::string_view message) {
    asio::async_write(m_session->sock, asio::buffer(&message[0], message.length()),
        [this](auto const& ec, auto const /*bytes_transferred*/) {
            if (ec)
                emit errorOccured(ec);
    });
}

void Client::handle_read() {
    m_readThread = std::make_unique<std::thread>(m_strand.wrap([this]() { read(); }));
}

void Client::read() {
    asio::async_read_until(m_session->sock, m_sbuf, '\n',
        [this](auto const& ec, auto const /*bytes_transferred*/) {
            if (!ec) {
                auto const message = Utility::make_string(m_sbuf);
                emit received(message);
                read();
            }
    });
}

void Client::setClientUsername(std::string_view username) {
    m_user_name = username;
}
