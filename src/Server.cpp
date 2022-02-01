/*
 * MIT License
 *
 * Copyright (c) 2022 Szymon Milewski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "Server.h"
#include "Service.h"

Server::Server() {}

void Server::start(asio::ip::address const& ip_address, unsigned short const port) {
    m_ioc.restart();
    m_port = port;

    m_work = std::make_unique<asio::io_context::work>(m_ioc);
    m_ep = std::make_unique<asio::ip::tcp::endpoint>(ip_address, port);
    m_acceptor = std::make_unique<asio::ip::tcp::acceptor>(m_ioc, *m_ep);

    m_acceptor->listen();
    startAccepting();

    for (auto _{ 0 }; _ < 2; ++_) {
        auto th{ std::make_unique<std::thread>([this]() { m_ioc.run(); }) };
        m_thread_pool.push_back(std::move(th));
    }
}

void Server::startAccepting() {
    auto session{ std::make_shared<ServerSession>(m_ioc, m_port) };

    m_acceptor->async_accept(session->sock,
        [this, session](auto const& ec) {
            onAccept(session, ec);
    });
}

void Server::onAccept(std::shared_ptr<ServerSession> session, const system::error_code &ec) {
    if (!ec) {
        m_active_sessions.push_back(session);
        auto service{ std::make_shared<Service>(session, m_active_sessions, m_room_name) };

        service->startHandling();
        startAccepting();
    }
}

void Server::setRoomName(std::string_view room_name) {
    m_room_name = room_name;
}

void Server::stop() {
    m_acceptor->close();
    m_ioc.stop();

    for (auto& th : m_thread_pool) {
        if (th->joinable())
            th->join();
    }
}

asio::ip::address Server::address() const noexcept {
    return m_ep->address();
}
