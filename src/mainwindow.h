#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "Server.h"
#include "Client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
                    MainWindow(QWidget *parent = nullptr);
                    ~MainWindow();

private slots:
    void            on_connectButton_clicked();
    void            on_hostButton_clicked();

    void            on_exitButton_clicked();
    void            on_backButton_2_clicked();

    void            on_connectButton_2_clicked();
    void            on_exitButton_2_clicked();

    void            on_backButton_3_clicked();
    void            on_startButton_clicked();

    void            on_exitButton_3_clicked();
    void            on_sendButton_clicked();

    void            onReceived(std::string_view);
    void            onErrorOccured(system::error_code const& ec);

    void            onEnterPressed();
    void            onConnected();

    void            onBadConnect(system::error_code const& ec);
    void            onReceivedInfo(std::pair<std::string, std::vector<std::string>> info);

private:
    void            sendMessage();
    void            badConnect(system::error_code const& ec);
    void            updateUserlist(std::vector<std::string> const& user_list);
    void            setupAppearance();
private:
    Ui::MainWindow *ui;
    std::unique_ptr<Client> client;
    std::unique_ptr<Server> server;
    bool hosted{false};
};
#endif // MAINWINDOW_H
