#ifndef CLIENT_H
#define CLIENT_H

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
#include "Client_Session.h"
#include "Utility.h"

using namespace boost;

class Client final : public QObject {
    Q_OBJECT

public:
                Client();
                ~Client();

    void        connect(asio::ip::address const& ip_address, unsigned short const port, Utility::Usertype ut);
    void        disconnect();

    void        setClientUsername(std::string_view username);
    void        communicate();

    void        sendMessage(std::string_view message);

    void        write(std::string_view message);
    void        read();

    void        onReadDone();

signals:
    void                messageReceived(std::vector<std::string>& message_set);
    void                serverMessageReceived(std::vector<std::string>& message_set);

    void                errorOccured(system::error_code const& ec);
    void                badConnect(system::error_code const& ec);

    void                connected();

private:
    void                        onConnected(system::error_code const& ec);
    std::vector<std::string>    processMessage(std::string_view message);

private:
    bool                                        m_connected{ false };

    asio::io_context                            m_ioc;
    std::unique_ptr<asio::io_context::work>     m_work;

    std::string                                 m_user_name;

    asio::streambuf                             m_read_sbuf;
    Utility::Usertype                           m_usertype;

    std::shared_ptr<ClientSession>              m_session;
    std::vector<std::unique_ptr<std::thread>>   m_thread_pool;
};
#endif // CLIENT_H
