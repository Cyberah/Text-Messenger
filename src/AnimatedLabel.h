#ifndef ANIMATEDLABEL_H
#define ANIMATEDLABEL_H

#include <QLabel>
#include <QPropertyAnimation>

class AnimatedLabel final : public QObject {
    Q_OBJECT
public:
    explicit                         AnimatedLabel(QObject *parent = nullptr) noexcept;

public:
    QPropertyAnimation*              createAnimation(QLabel *);

private:
    int                              m_width;
    int                              m_height;
};

#endif // ANIMATEDLABEL_H
