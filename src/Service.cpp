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

#include "Service.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

Service::Service(std::shared_ptr<ServerSession> session, std::vector<std::shared_ptr<ServerSession>> &active_sessions, std::string_view room_name)
    : m_session{ session }
    , m_active_sessions{ active_sessions }
    , m_room_name{ room_name } {

}

void Service::startHandling() {
    readData();
}

void Service::readData() {
    auto self{ shared_from_this() };

    asio::async_read_until(m_session->sock, m_read_sbuf, '\n',
        [this, self](auto const& ec, auto const /*bytes_received*/) {
            onReadDone(ec);
    });
}

void Service::onReadDone(system::error_code const& ec) {
    if (!ec) {
        auto const message_raw{ Utility::makeString(m_read_sbuf) };
        processMessage(message_raw);

        ServerMessage const complete_message{ ServerProperties{ m_message_type, m_room_name, userListToString(), m_session->username, m_usertype }, m_message };
        sendToAll(complete_message());
    }
}

void Service::sendToAll(std::string_view message) {
    for (auto const& s : m_active_sessions) {
        asio::async_write(s->sock, asio::buffer(&message[0], message.size()),
            [](auto const/*& ec*/, auto const /*bytes_sent*/) {
                //NO-OP
        });
    }
    readData();
}

std::string Service::userListToString() {
    auto users_str{ std::accumulate(m_active_sessions.cbegin(), m_active_sessions.cend(), std::string{},
        [](auto const& str, auto const& session) {
            return str + session->username + ',';
        }) };

    users_str.pop_back();

    return users_str;
}

void Service::processMessage(std::string_view message) {
    std::vector<std::string> result;
    boost::split(result, message, boost::is_any_of("|\\"), boost::token_compress_on);

    m_message_type = MessageTypeConvertions::strToMessageType(result[0]);
    m_session->username = result[1];
    m_usertype = strToUsertype(result[2]);
    m_message = result[3];

}

Utility::Usertype Service::strToUsertype(std::string_view user_type) {
    return user_type == "ADMIN" ? Utility::Usertype::ADMIN : Utility::Usertype::USER;
}
