#ifndef SERVICE_H
#define SERVICE_H
#include "Server_Session.h"
#include "Utility.h"

class Service final : public std::enable_shared_from_this<Service> {
public:
    explicit        Service(std::shared_ptr<ServerSession> s, std::vector<std::shared_ptr<ServerSession>>& active_sessions, std::string_view room_name);
    void            startHandling();

private:
    void            receive_info();
    void            on_received_info(system::error_code const& ec);

    void            write_info();

    void            readData();
    void            onReadDone(system::error_code const& ec);

    void            sendToAll(std::string_view message);

    std::string     make_message(std::string_view message);
    std::string     user_list_str();

private:
    std::string message;
    asio::streambuf m_read_sbuf;
    asio::streambuf m_info_sbuf;
    std::shared_ptr<ServerSession> m_session;
    Utility::Usertype m_usertype;
    std::vector<std::shared_ptr<ServerSession>>& m_active_sessions;
    std::string const m_room_name;
};

#endif // SERVICE_H
