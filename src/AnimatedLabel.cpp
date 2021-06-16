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
