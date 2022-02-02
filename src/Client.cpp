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
#include "Client.h"
#include "Client_Session.h"
#include "Message.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

Client::Client() {}

Client::~Client() {
    if (m_connected)
        stop();
}

void Client::stop() {
    m_ioc.stop();
    for (auto& th : m_thread_pool) {
        if (th->joinable()) {
            th->join();
        }
    }
    m_thread_pool.clear();
}

void Client::connect(asio::ip::address const& ip_address, unsigned short const port, Utility::Usertype ut) {
    m_ioc.restart();
    m_work = std::make_unique<asio::io_context::work>(m_ioc);

    m_session = std::make_unique<ClientSession>(m_ioc, ip_address, port);
    m_usertype = ut;

    m_session->sock.async_connect(m_session->ep,
        [this](auto const& ec) {
            onConnected(ec);
    });

    for (auto i{ 0 }; i < 2; ++i) {
        auto th{ std::make_unique<std::thread>([this]() { m_ioc.run(); }) };
        m_thread_pool.push_back(std::move(th));
    }
}

void Client::disconnect() {
    if (m_connected) {
        ClientMessage const user_left{ ClientProperties{ MessageType::USER_LEFT, m_user_name, m_usertype }, "" };
        write(user_left());

        shutdownConnection();
        stop();
    }
}

void Client::shutdownConnection() {
    m_connected = false;
    m_session->sock.shutdown(asio::ip::tcp::socket::shutdown_both);
}

void Client::onConnected(system::error_code const& ec) {
    if (!ec) {
        m_connected = true;
        emit connected();
    }

    else
        emit badConnect(ec);
}

void Client::communicate() {
    ClientMessage const user_connected{ ClientProperties{ MessageType::USER_CONNECTED, m_user_name, m_usertype }, "blank" };

    write(user_connected());
    read();

}

void Client::sendMessage(std::string_view message) {
    ClientMessage const complete_message{ ClientProperties{ MessageType::NORMAL, m_user_name, m_usertype }, MessageFilter::filterBackSlashes(message) };
    write(complete_message());
}

void Client::write(std::string_view message) {
    asio::async_write(m_session->sock, asio::buffer(&message[0], message.length()),
        [this](auto const& ec, auto const /*bytes_transferred*/) {
            if (ec)
                emit errorOccured(ec);
    });
}

void Client::read() {
    asio::async_read_until(m_session->sock, m_read_sbuf, '\n',
        [this](auto const& ec, auto const /*bytes_transferred*/) {
            if (!ec) {
                onReadDone();
                read();
            }
    });
}

void Client::onReadDone() {
    auto const message_raw{ Utility::makeString(m_read_sbuf) };
    std::vector<std::string> result{ processMessage(message_raw) };

    if (MessageTypeConvertions::strToMessageType(result[0]) == MessageType::NORMAL)
        emit messageReceived(result);

    else
        emit serverMessageReceived(result);

}

std::vector<std::string> Client::processMessage(std::string_view message) {
    std::vector<std::string> result;
    boost::split(result, message, boost::is_any_of("|\\"), boost::token_compress_on);
    return result;
}

void Client::setClientUsername(std::string_view username) {
    m_user_name = username;
}
