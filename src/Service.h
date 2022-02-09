#ifndef SERVICE_H
#define SERVICE_H

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

#include "Server_Session.h"
#include "Message.h"

class Service final : public std::enable_shared_from_this<Service> {
public:
    explicit            Service(std::shared_ptr<ServerSession> session, std::vector<std::shared_ptr<ServerSession>>& active_sessions, std::string_view room_name);
    void                startHandling();

private:
    void                        readData();
    void                        onReadDone(system::error_code const& ec);

    void                        sendToAll(std::string_view message);
    void                        processMessage(std::string_view message);

    std::string                 userListToString();
    void                        removeSession(std::string_view username);

private:
    asio::streambuf                                 m_read_sbuf;

    std::shared_ptr<ServerSession>                  m_session;
    std::vector<std::shared_ptr<ServerSession>>&    m_active_sessions;

    Utility::Usertype                               m_usertype;
    MessageType                                     m_message_type;

    std::string                                     m_message;

    std::string                                     m_user_name;
    std::string                                     m_room_name;
};

#endif // SERVICE_H
