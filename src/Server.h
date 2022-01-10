#ifndef SERVER_H
#define SERVER_H

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

#include <QObject>
#include "Server_Session.h"
#include "Service.h"

class Server {
public:
                      Server();

    void              start(asio::ip::address const& ip_address, unsigned short const port);
    void              startAccepting();

    void              onAccept(std::shared_ptr<ServerSession> session, system::error_code const& ec);
    void              setRoomName(std::string_view room_name);

    void              stop();
    asio::ip::address address() const noexcept;

private:
    unsigned short          m_port;
    asio::io_context        m_ioc;
    std::string             m_room_name;

    std::unique_ptr<asio::ip::tcp::endpoint>    m_ep;
    std::unique_ptr<asio::io_context::work>     m_work;
    std::unique_ptr<asio::ip::tcp::acceptor>    m_acceptor;
    std::vector<std::shared_ptr<ServerSession>> m_active_sessions;
    std::vector<std::unique_ptr<std::thread>>   m_thread_pool;
};

#endif // SERVER_H
