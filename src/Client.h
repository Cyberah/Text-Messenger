#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include "Client_Session.h"

using namespace boost;

class Client final : public QObject {
    Q_OBJECT
public:
    Client();

    void connect(asio::ip::address const& ip_address, unsigned short const port);
    void disconnect();
    void setClientUsername(std::string_view username);
    void write(std::string_view message);
    void read();

signals:
    void received(std::string_view msg);
    void errorOccured(system::error_code const& ec);

    void badConnect(system::error_code const& ec);
    void connected();

private:
    void on_connected(system::error_code const& ec);
    void write_username();

    void handle_write();
    void handle_read();


    std::string get_message();

private:
    asio::io_context m_ioc;
    std::unique_ptr<asio::io_context::work> m_work;
    std::string m_user_name{""};
    asio::streambuf m_sbuf;
    std::shared_ptr<ClientSession> m_session;
    std::unique_ptr<std::thread> m_readThread;
    asio::io_context::strand m_strand{ m_ioc };
    std::vector<std::unique_ptr<std::thread>> m_thread_pool;
};

#endif // CLIENT_H
