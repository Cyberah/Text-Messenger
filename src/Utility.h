#ifndef UTILITY_H
#define UTILITY_H
#include <boost/asio.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <QObject>
#include <QAbstractButton>
#include <QPixmap>
#include <QIcon>

namespace Utility {
    enum class Usertype : std::size_t {
        ADMIN,
        USER
    };

    inline std::string make_string(boost::asio::streambuf& buf) {
        std::string temp;
        std::istream is{ &buf };

        std::getline(is, temp);
        return temp;
    }

    inline void set_button_icon(QAbstractButton* button, QString const& image_path, double const factor) {
        QPixmap pixmap(image_path);
        QIcon ButtonIcon(pixmap);

        button->setIcon(ButtonIcon);
        button->setIconSize(button->rect().size() / factor);
    }

    //process message: username|\\usertyope|\\message|\\userlist
    inline std::pair<std::string, std::vector<std::string>> process_message(std::string_view message) {
        std::vector<std::string> result;
        boost::split(result, message, boost::is_any_of("|\\"), boost::token_compress_on);

        std::string const processed_message{ result[0] + "(" + result[1] + "): " + result[2] };

        std::vector<std::string> user_list;
        boost::split(user_list, result[3], boost::is_any_of(","));

        return { processed_message, user_list };
    }

    //process server info: roomname|\\userlist
    inline std::pair<std::string, std::vector<std::string>> process_server_info(std::string_view info) {
        std::vector<std::string> result;
        boost::split(result, info, boost::is_any_of("|\\"), boost::token_compress_on);

        auto const room_name{result[0]};

        std::vector<std::string> user_list;
        boost::split(user_list, result[1], boost::is_any_of(","));

        return { room_name, user_list };
    }

    //username|\\usertype;
    inline std::pair<std::string, Usertype> extract_user_info(std::string_view content) {
        auto const pos{ content.find("|\\") };

        std::string const username{ content.cbegin(), content.cbegin() + pos };
        std::string const usertype_str{ content.cbegin() + pos + 2, content.cend() };

        auto const usertype{ usertype_str == "ADMIN" ? Usertype::ADMIN : Usertype::USER };
        return { username, usertype };
    }

    /*inline void setButtonAppearance(QAbstractButton* button) {
        button->setStyleSheet("QPushButton { font: 24pt; border: 2px solid #ffd700; border-radius: 12px; } QPushButton:hover { background-color: #053563; }");
    }*/
}

#endif // UTILITY_H
