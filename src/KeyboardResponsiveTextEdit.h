#ifndef KEYBOARDRESPONSIVETEXTEDIT_H
#define KEYBOARDRESPONSIVETEXTEDIT_H
#include <QPlainTextEdit>
#include <QKeyEvent>

class KeyboardResponsiveTextEdit final : public QPlainTextEdit {
    Q_OBJECT
public:
    KeyboardResponsiveTextEdit(QWidget* parent = nullptr);
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void enterPressed();
private:
    //stuff?
};

#endif // KEYBOARDRESPONSIVETEXTEDIT_H
