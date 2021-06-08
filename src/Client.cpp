#include "Client.h"
#include "Client_Session.h"

Client::Client() {}

void Client::connect(asio::ip::address const& ip_address, const unsigned short port, Utility::Usertype ut) {
    m_work = std::make_unique<asio::io_context::work>(m_ioc);
    m_session = std::make_shared<ClientSession>(m_ioc, ip_address, port);
    m_usertype = ut;

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

    if (m_readThread->joinable())
        m_readThread->join();

    for (auto& th : m_thread_pool)
        th->join();
}

void Client::on_connected(system::error_code const& ec) {
    if (!ec)
        emit connected();
    else
        emit badConnect(ec);
}

void Client::communicate() {
    write_info();
}

void Client::write_info() {
    auto const ut{ m_usertype == Utility::Usertype::ADMIN ? "ADMIN" : "USER" };
    auto const complete_info{ m_user_name + "|\\" + ut + '\n' };

    asio::async_write(m_session->sock, asio::buffer(&complete_info[0], complete_info.length()),
        [this](auto const& ec, auto const /*bytes_transferred*/) {
            if (!ec)
                read_info();
            else
                emit errorOccured(ec);
    });
}

void Client::read_info() {
    asio::async_read_until(m_session->sock, m_info_sbuf, '\n',
        [this](auto const& ec, auto const /*bytes_transferred*/) {
            if (!ec) {
                auto const info_raw = Utility::make_string(m_info_sbuf);
                auto const info{ Utility::process_server_info(info_raw) };

                emit received_info(info);
                handle_read();
            }
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
    asio::async_read_until(m_session->sock, m_read_sbuf, '\n',
        [this](auto const& ec, auto const /*bytes_transferred*/) {
            if (!ec) {
                auto const message = Utility::make_string(m_read_sbuf);
                emit received(message);

                read();
            }
    });
}

void Client::setClientUsername(std::string_view username) {
    m_user_name = username;
}
