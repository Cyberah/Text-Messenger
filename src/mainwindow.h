#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#include <QMainWindow>
#include <QKeyEvent>
#include "Server.h"
#include "Client.h"
#include "AnimatedLabel.h"

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

    void            on_exitLobbyButton_clicked();
    void            on_leaveLobbyButton_clicked();

    void            onMessageReceived(std::vector<std::string>& message_set);
    void            onErrorOccured(system::error_code const& ec);

    void            onEnterPressed();
    void            onConnected();

    void            onBadConnect(system::error_code const& ec);
    void            onServerMessageReceived(std::vector<std::string>& message_set);

private:
    void                        sendMessage();
    void                        badConnect(system::error_code const& ec);

    void                        updateUserlist(std::vector<std::string> const& user_list);
    void                        setupAppearance();

    void                        clearWidgets();
    void                        clearInputs();

    void                        updateRoomName(std::string_view room_name);
    void                        onAdminLeave();

    std::vector<std::string>    strToVectorUserlist(std::string_view user_list_str);
    void                        connectClient(std::unique_ptr<Client>&& client);

private:
    Ui::MainWindow*             ui;
    AnimatedLabel*              animatedLabelFactory;

    std::unique_ptr<Client>     client;
    std::unique_ptr<Server>     server;

    bool                        hosted{ false };
    bool                        connected{ false };
};
#endif // MAINWINDOW_H
