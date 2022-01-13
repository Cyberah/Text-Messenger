#ifndef MESSAGE_TEST_H
#define MESSAGE_TEST_H

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
 * LIABILITY, WHETHtER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <gtest/gtest.h>

#include "../src/Message.h"
#include "../src/Utility.h"

using namespace testing;

TEST(MessageTest, ClientMessageReturnsProperString) {
    ClientMessage const client_message_test{ ClientProperties{ MessageType::NORMAL, "Username", Utility::Usertype::USER }, "Some message" };
    auto const client_message_str_pattern{ "NORMAL|\\Username|\\USER|\\Some message\n" };

    EXPECT_EQ(client_message_test(), client_message_str_pattern);
}

TEST(MessageTest, ServerMessageReturnsProperString) {
    ServerMessage const server_message_test{ ServerProperties{ MessageType::USER_CONNECTED, "Room Name", "user1,user2,user3", "Username", Utility::Usertype::ADMIN }, "Some message" };
    auto const server_message_str_pattern{ "USER_CONNECTED|\\Room Name|\\user1,user2,user3|\\Username|\\ADMIN|\\Some message\n"};

    EXPECT_EQ(server_message_test(), server_message_str_pattern);
}

TEST(MessageTest, EmptyMessageNotIncludedInString) {
    ServerMessage const server_message_test{ ServerProperties{ MessageType::USER_LEFT, "Room Name", "user1,user2,user3", "Username", Utility::Usertype::ADMIN }, "" };
    auto const server_message_str_pattern{ "USER_LEFT|\\Room Name|\\user1,user2,user3|\\Username|\\ADMIN|\\\n" };

    EXPECT_EQ(server_message_test(), server_message_str_pattern);
}

TEST(MessageTest, NoRoomnameAndUsernameInServerMessage) {
    ServerMessage const server_message_test{ ServerProperties{ MessageType::NORMAL, "", "user1,user2,user3", "", Utility::Usertype::ADMIN }, "Some message" };
    auto const server_message_str_pattern{ "NORMAL|\\|\\user1,user2,user3|\\|\\ADMIN|\\Some message\n" };

    EXPECT_EQ(server_message_test(), server_message_str_pattern);
}

TEST(MessageTest, FilterRemovesBackSlashes) {
    auto const filtered{ MessageFilter::filterBackSlashes("Some\\ generic \\message") };
    auto const expected{"Some generic message"};

    EXPECT_EQ(filtered, expected);
}

//no further tests are needed since this unit has very limited use case.

#endif // MESSAGE_TEST_H
