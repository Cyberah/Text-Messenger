#include "Service.h"
#include "Utility.h"

Service::Service(std::shared_ptr<ServerSession> s, std::vector<std::shared_ptr<ServerSession>> &active_sessions, std::string_view room_name)
    : m_session{ s }
    , m_active_sessions{ active_sessions }
    , m_room_name{ room_name } {

}

void Service::startHandling() {
    receive_info();
}

void Service::receive_info() {
    auto self{ shared_from_this() };

    asio::async_read_until(m_session->sock, m_info_sbuf, '\n',
        [this, self] (auto const& ec, auto const /*bytes_received*/) {
            on_received_info(ec);
    });
}

void Service::on_received_info(system::error_code const& ec) {
    if (!ec) {
        auto received{ Utility::make_string(m_info_sbuf) };
        auto const[user_name, user_type]{ Utility::extract_user_info(received) };

        m_session->username = user_name;
        m_usertype = user_type;

        write_info();
    }
}

void Service::write_info() {
     auto self{ shared_from_this() };

     auto const complete_info{ m_room_name + "|\\" + user_list_str() + '\n' };

     asio::async_write(m_session->sock, asio::buffer(&complete_info[0], complete_info.length()),
         [this, self](auto const& ec, auto const /*bytes_received*/) {
             if (!ec)
                 sendToAll("S" + m_session->username + '\n');
     });
}

void Service::readData() {
    auto self{ shared_from_this() };

    asio::async_read_until(m_session->sock, m_read_sbuf, '\n',
        [this, self](auto const& ec, auto const /*bytes_received*/) {
            onReadDone(ec);
    });
}

void Service::onReadDone(const system::error_code &ec) {
    if (!ec) {
        message = Utility::make_string(m_read_sbuf);

        auto const complete_message{'M' + make_message(message)};

        sendToAll(complete_message);
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

std::string Service::make_message(std::string_view message) {
    auto const ut{ m_usertype == Utility::Usertype::ADMIN ? "ADMIN" : "USER" };

    auto const users_str{ user_list_str() };

    return m_session->username + "|\\" + ut + "|\\" + message.data() + "|\\" + users_str + '\n';
}

 std::string Service::user_list_str() {
    auto users_str{ std::accumulate(m_active_sessions.cbegin(), m_active_sessions.cend(), std::string{},
        [](auto const& str, auto const& session) {
            return str + session->username + ',';
        }) };

    users_str.pop_back();

    return users_str;
}
