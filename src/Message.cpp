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

#include "Message.h"

ClientMessage::ClientMessage(ClientProperties const& properties, std::string_view message)
    : m_properties{ properties }
    , m_message{ message } {}

std::string ClientMessage::operator()() const {
    auto const mt{ MessageTypeConvertions::messageTypeToString(m_properties.message_type) };
    std::string const ut{ m_properties.user_type == Utility::Usertype::ADMIN ? "ADMIN" : "USER" };

    return mt
        + "|\\" + m_properties.user_name
        + "|\\" + ut
        + "|\\" + m_message
        + '\n';
}

ServerMessage::ServerMessage(ServerProperties const& properties, std::string_view message)
    : m_properties{ properties }
    , m_message{ message } {}

std::string ServerMessage::operator()() const {
    auto const mt{ MessageTypeConvertions::messageTypeToString(m_properties.message_type) };
    std::string const ut{ m_properties.user_type == Utility::Usertype::ADMIN ? "ADMIN" : "USER" };

    return mt
        + "|\\" + m_properties.room_name
        + "|\\" + m_properties.user_list
        + "|\\" + m_properties.user_name
        + "|\\" + ut
        + "|\\" + m_message
        + '\n';
}
