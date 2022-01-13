#ifndef MESSAGE_H
#define MESSAGE_H

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

#include "Utility.h"

enum class MessageType {
    USER_CONNECTED,
    NORMAL,
    USER_LEFT
};

struct ClientProperties final {
    explicit ClientProperties(MessageType const message_type, std::string_view user_name, Utility::Usertype const user_type)
        : message_type{ message_type }
        , user_name{ user_name }
        , user_type{ user_type } {}

    MessageType const message_type;
    std::string const user_name;
    Utility::Usertype const user_type;
};

struct ServerProperties final {
    explicit ServerProperties(MessageType const message_type, std::string_view room_name, std::string_view user_list, std::string_view user_name, Utility::Usertype const user_type)
        : message_type{ message_type }
        , room_name{ room_name }
        , user_list{ user_list }
        , user_name{ user_name }
        , user_type{ user_type } {}

    MessageType const message_type;
    std::string const room_name;
    std::string const user_list;
    std::string const user_name;
    Utility::Usertype const user_type;
};

class IMessage {
public:
    virtual std::string operator()() const = 0;
};

class ClientMessage final : public IMessage {
public:
    explicit ClientMessage(ClientProperties const& properties, std::string_view message);
    std::string operator()() const override;

private:
    ClientProperties const  m_properties;
    std::string const       m_message;
};


class ServerMessage final : public IMessage {
public:
    explicit ServerMessage(ServerProperties const& properties, std::string_view message);
    std::string operator()() const override;

private:
    ServerProperties const  m_properties;
    std::string const       m_message;
};

namespace MessageTypeConvertions {
    inline std::string messageTypeToString(MessageType const message_type) {
        std::string type;
        switch(message_type) {
            case MessageType::NORMAL:
                type = "NORMAL";
                break;

            case MessageType::USER_CONNECTED:
                type = "USER_CONNECTED";
                break;

            case MessageType::USER_LEFT:
                type = "USER_LEFT";
                break;

            default:
                break;
        }
        return type;
    }

    inline MessageType strToMessageType(std::string_view message_type) {
        MessageType type;
        if (message_type == "USER_CONNECTED")
            type = MessageType::USER_CONNECTED;

        else if (message_type == "USER_LEFT")
            type = MessageType::USER_LEFT;

        else
            type = MessageType::NORMAL;

        return type;
    }
}

namespace MessageFilter {
    inline std::string filterBackSlashes(std::string_view content) {
        std::string temp{ content };
        temp.erase(std::remove(temp.begin(), temp.end(), '\\'), temp.end());
        temp.shrink_to_fit();
        return temp;
    }
}
#endif // MESSAGE_H
