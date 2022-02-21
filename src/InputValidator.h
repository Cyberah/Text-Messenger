#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

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

#include <QString>

class InputValidator final {
public:
    InputValidator() = delete;
    inline static bool validateInput(QString const& input) {
        if (input.size() == 0)
            return true;

        else
            return std::any_of(input.cbegin(), input.cend(),
                [](auto const c) {
                    return c == '\\' || c == ',';
                });
    }

    inline static bool validatePort(QString const& port) {
        if (port.size() == 0 || port.size() > 5)
            return true;

        auto const std_port{ port.toStdString() };
        return std::any_of(std_port.cbegin(), std_port.cend(),
                           [](auto const c) {
                               return !std::isdigit(c);
                           });
    }
};

#endif // INPUTVALIDATOR_H
