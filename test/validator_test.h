#ifndef VALIDATOR_TEST_H
#define VALIDATOR_TEST_H

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

#include "../src/InputValidator.h"

using namespace testing;

TEST(InputValidatorInputTest, PassingEmptyInputReturnsTrue) {
    QString const sample{""};
    auto const result{InputValidator::validateInput(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorInputTest, PassingCommaReturnsTrue) {
    QString const sample{","};
    auto const result{InputValidator::validateInput(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorInputTest, PassingProperLongSentenceReturnsFalse) {
    QString const sample{"This is some loooooong sentence that user might enter!"};
    auto const result{InputValidator::validateInput(sample)};

    ASSERT_FALSE(result);
}

TEST(InputValidatorInputTest, PassingCommasBetweenCharactersReturnsTrue) {
    QString const sample{"I, wanted, to, have, so, many, commas, in, my, username, BUT, it, didn't work!"};
    auto const result{InputValidator::validateInput(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorInputTest, PassingExclusivelyCommasReturnsTrue) {
    QString const sample{",,,,,,,,,,,,,,,,,,,,,,,,"};
    auto const result{InputValidator::validateInput(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorInputTest, PassingBackslashReturnsTrue) {
    QString const sample{"\\"};
    auto const result{InputValidator::validateInput(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorInputTest, PassingMultipleBackslashesReturnsTrue) {
    QString const sample{"\\\\\\\\\\\\\\\\\\\\\\\\\\\\"};
    auto const result{InputValidator::validateInput(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorInputTest, PassingBackslashesBetweenCharactersReturnsTrue) {
    QString const sample{"I\\want\\to\\break\\it\\so\\much\\"};
    auto const result{InputValidator::validateInput(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorInputTest, PassingBackslashesAndCommasReturnsTrue) {
    QString const sample{",\\,,\\,\\,,,\\\\\\,,\\"};
    auto const result{InputValidator::validateInput(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorPortTest, PassingEmptyPortReturnsTrue) {
    QString const sample{""};
    auto const result{InputValidator::validatePort(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorPortTest, PassingProperPortReturnsFalse) {
    QString const sample{"3333"};
    auto const result{InputValidator::validatePort(sample)};

    ASSERT_FALSE(result);
}

TEST(InputValidatorPortTest, PassingCharacterReturnsTrue) {
    QString const sample{"C"};
    auto const result{InputValidator::validatePort(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorPortTest, PassingMultipleCharactersReturnsTrue) {
    QString const sample{"port"};
    auto const result{InputValidator::validatePort(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorPortTest, PassingCharactersAndDigitsMixedReturnsTrue) {
    QString const sample{"p0rt123nope643"};
    auto const result{InputValidator::validatePort(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorPortTest, PassingMoreThanFiveDigitsReturnsTrue) {
    QString const sample{"655366"};
    auto const result{InputValidator::validatePort(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorPortTest, PassingSpecialCharactersReturnsTrue) {
    QString const sample{"\\!@,,#"};
    auto const result{InputValidator::validatePort(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorPortTest, PassingSpecialCharactersAndDigitsMixedReturnsTrue) {
    QString const sample{"33,33"};
    auto const result{InputValidator::validatePort(sample)};

    ASSERT_TRUE(result);
}

TEST(InputValidatorPortTest, PassingNegativePortNumberReturnsTrue) {
    QString const sample{"-25565"};
    auto const result{InputValidator::validatePort(sample)};

    ASSERT_TRUE(result);
}
#endif // VALIDATOR_TEST_H
