#include "Service.h"
#include "Utility.h"

Service::Service(std::shared_ptr<ServerSession> s, std::vector<std::shared_ptr<ServerSession>> &active_sessions)
    : m_session{ s }
    , m_active_sessions{ active_sessions } {

}

void Service::startHandling() {
    receiveUserName();
}

void Service::receiveUserName() {
    auto self{ shared_from_this() };
    asio::async_read_until(m_session->sock, m_sbuf, '\n',
        [this, self] (auto const& ec, auto const /*bytes_received*/) {
            onReceivedUserName(ec);
    });
}

void Service::onReceivedUserName(system::error_code const& ec) {
    if (!ec) {
        m_session->user_name = Utility::make_string(m_sbuf);
        readData();
    }
}

void Service::readData() {
    auto self{ shared_from_this() };
    asio::async_read_until(m_session->sock, m_sbuf, '\n',
        [this, self](auto const& ec, auto const /*bytes_received*/) {
            onReadDone(ec);
    });
}

void Service::onReadDone(const system::error_code &ec) {
    if (!ec) {
        message = Utility::make_string(m_sbuf);
        if (message == "!end") {
            m_active_sessions.erase(std::find(m_active_sessions.cbegin(), m_active_sessions.cend(), m_session));
            m_session->sock.close();
            return;
        }
        sendToAll(message);
    }
}

void Service::sendToAll(const std::string &message) {
    auto self{ shared_from_this() };
    auto const complete_message{m_session->user_name + " >> " + message + '\n'};
    for (auto const& s : m_active_sessions) {
        asio::async_write(s->sock, asio::buffer(&complete_message[0], complete_message.size()),
                [this, self](auto const& ec, auto const /*bytes_sent*/) {
                    onSentToAll(ec);
        });
    }
}

void Service::onSentToAll(const system::error_code &ec) {
    if (ec) {
        //std::cout << "onSentToAll() error occured: " << ec.message() << '\t' << ec.value() << '\n';
        return;
    }
    readData();
}
