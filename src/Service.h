#ifndef SERVICE_H
#define SERVICE_H
#include "Server_Session.h"

class Service final : public std::enable_shared_from_this<Service> {
public:
    Service(std::shared_ptr<ServerSession> s, std::vector<std::shared_ptr<ServerSession>>& active_sessions);
    void startHandling();

private:
    void receiveUserName();
    void onReceivedUserName(system::error_code const& ec);

    void readData();
    void onReadDone(system::error_code const& ec);

    void sendToAll(std::string const& message); //change to sv
    void onSentToAll(system::error_code const& ec);

private:
    std::string message;
    asio::streambuf m_sbuf;
    std::shared_ptr<ServerSession> m_session;
    std::vector<std::shared_ptr<ServerSession>>& m_active_sessions;
};

#endif // SERVICE_H
