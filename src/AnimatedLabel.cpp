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

#include "AnimatedLabel.h"

AnimatedLabel::AnimatedLabel(QObject *parent) noexcept
    : QObject(parent) {}

QPropertyAnimation *AnimatedLabel::createAnimation(QLabel *label) {
    m_height = label->height();
    m_width = label->width();

    label->setText("Don't forget to check out source code on my github site! https://github.com/Cyberah/Text-Messenger");
    label->setFixedHeight(30);

    label->setFixedWidth(1000);
    label->setStyleSheet("background: none;");

    QPropertyAnimation *animation = new QPropertyAnimation(label, "geometry");
    animation->setDuration(45000);

    animation->setStartValue(QRect((m_width + 2500), m_height, 100, 100));
    animation->setEndValue(QRect((m_width - m_width - 3000), m_height, 100, 100));

    animation->setLoopCount(-1);
    animation->start();

    return animation;
}
