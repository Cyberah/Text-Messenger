#ifndef UTILITY_H
#define UTILITY_H

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

#include <boost/asio.hpp>
#include <QObject>
#include <QAbstractButton>
#include <QPixmap>
#include <QIcon>

namespace Utility {
    enum class Usertype : std::size_t {
        ADMIN,
        USER
    };

    inline std::string makeString(boost::asio::streambuf& buf) {
        std::string temp;
        std::istream is{ &buf };

        std::getline(is, temp);
        return temp;
    }

    inline void setButtonIcon(QAbstractButton* button, QString const& image_path, double const factor) {
        QPixmap pixmap(image_path);
        QIcon ButtonIcon(pixmap);

        button->setIcon(ButtonIcon);
        button->setIconSize(button->rect().size() / factor);
    }

    inline Utility::Usertype strToUsertype(std::string_view user_type) {
        return user_type == "ADMIN" ? Utility::Usertype::ADMIN : Utility::Usertype::USER;
    }
}

#endif // UTILITY_H
