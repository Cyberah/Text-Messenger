#ifndef UTILITY_H
#define UTILITY_H
#include <boost/asio.hpp>
#include <QObject>
#include <QAbstractButton>
#include <QPixmap>
#include <QIcon>

namespace Utility {
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
}

#endif // UTILITY_H
