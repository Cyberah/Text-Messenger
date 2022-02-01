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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Utility.h"
#include "KeyboardResponsiveTextEdit.h"

#include <QString>
#include <QMessageBox>
#include <QLabel>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->inputTextEdit->setPlaceholderText("Type here");
    ui->chatTextWidget->setReadOnly(true);

    animatedLabelFactory = new AnimatedLabel{this};
    animatedLabelFactory->createAnimation(ui->animatedLabel);

    setupAppearance();

    Utility::setButtonIcon(ui->emojiButton, ":/img/img/emoji.png", 1.5);
    Utility::setButtonIcon(ui->attachmentsButton, ":/img/img/clip.png", 1.5);
    Utility::setButtonIcon(ui->sendButton, ":/img/img/arrow.png", 1.1);

    client = std::make_unique<Client>();
    server = std::make_unique<Server>();

    connect(client.get(), &Client::messageReceived, this, &MainWindow::onMessageReceived, Qt::BlockingQueuedConnection);
    connect(client.get(), &Client::errorOccured, this, &MainWindow::onErrorOccured, Qt::BlockingQueuedConnection);
    connect(client.get(), &Client::connected, this, &MainWindow::onConnected, Qt::BlockingQueuedConnection);
    connect(client.get(), &Client::badConnect, this, &MainWindow::onBadConnect, Qt::BlockingQueuedConnection);
    connect(client.get(), &Client::serverMessageReceived, this, &MainWindow::onServerMessageReceived, Qt::BlockingQueuedConnection);

    connect(ui->inputTextEdit, &KeyboardResponsiveTextEdit::enterPressed, this, &MainWindow::onEnterPressed);
}

MainWindow::~MainWindow() {
    if (connected)
        client->disconnect();

    if (hosted)
        server->stop();

    delete ui;
}

void MainWindow::on_connectButton_clicked() {
    ui->stackedWidget_2->setCurrentIndex(1);
}

void MainWindow::on_hostButton_clicked() {
    ui->stackedWidget_2->setCurrentIndex(2);
}

void MainWindow::on_exitButton_clicked() {
    QApplication::quit();
}

void MainWindow::on_backButton_2_clicked() {
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_connectButton_2_clicked() {
    ui->connection_status_label->setText("Connecting to " + ui->ip_address_le->text() + "...");
    auto const ip_address_raw{ui->ip_address_le->text().toStdString()};
    auto const port{ui->port_le->text().toShort()};

    system::error_code bad_address;
    auto const ip_address{ boost::asio::ip::address::from_string(ip_address_raw, bad_address) };

    if (!bad_address)
        client->connect(ip_address, port, Utility::Usertype::USER);
}

void MainWindow::on_exitButton_2_clicked() {
    QApplication::quit();
}

void MainWindow::on_backButton_3_clicked() {
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_startButton_clicked() {
    auto const port{ui->server_port_le->text().toShort()};
    auto const address_raw{ui->ip_address_le_2->text().toStdString()};
    auto const room_name{ui->room_name_le->text().toStdString()};

    system::error_code bad_address;
    auto const address{asio::ip::address::from_string(address_raw, bad_address)};

    if (!bad_address) {
        try {
            server->start(address, port);
            server->setRoomName(room_name);

            hosted = true;
            owned = true;
            ui->username_le->setText(ui->host_username_le->text());

            client->connect(server->address(), port, Utility::Usertype::ADMIN);
        }
        catch(system::system_error const& error) {
            auto errorMsgBox{new QMessageBox{this}};
            errorMsgBox->setText("Couldn't start server: " + QString::fromStdString(error.what()));

            errorMsgBox->exec();
        }
    }
    else
        ui->bad_ip_address_label->setText("Bad ip address!");
}

void MainWindow::on_exitButton_3_clicked() {
    QApplication::quit();
}

void MainWindow::on_sendButton_clicked() {
    sendMessage();
}

void MainWindow::onMessageReceived(std::vector<std::string>& message_set) {
    auto const room_name{ message_set[1] };
    auto const user_list{ message_set[2] };
    auto const user_name{ message_set[3] };
    auto const user_type{ message_set[4] };
    auto const message{ message_set[5] };

    updateRoomName(room_name);
    updateUserlist(strToVectorUserlist(user_list));

    ui->chatTextWidget->appendPlainText(QString::fromStdString(user_name + "(" + user_type + "): " + message));
}

void MainWindow::onErrorOccured(system::error_code const& ec) {
    auto errorMsgBox{new QMessageBox{this}};
    errorMsgBox->setText("Error occured: " + QString::fromStdString(ec.message()) + "\nError code: " + QString::number(ec.value()));

    errorMsgBox->exec();
}

void MainWindow::onEnterPressed() {
    sendMessage();
}

void MainWindow::sendMessage() {
    if (!ui->inputTextEdit->toPlainText().isEmpty()) {
        auto const message{ui->inputTextEdit->toPlainText().toStdString()};

        client->sendMessage(message);
        ui->inputTextEdit->clear();
    }
}

void MainWindow::updateUserlist(std::vector<std::string> const& user_list) {
    ui->userlistWidget->clear();
    for (auto const& user : user_list)
        ui->userlistWidget->addItem(QString::fromStdString(user));
}

void MainWindow::onConnected() {
    client->setClientUsername(ui->username_le->text().toStdString());
    client->communicate();

    connected = true;
    ui->username_label->setText(ui->username_le->text());
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::onBadConnect(system::error_code const& ec) {
    QString reason;
    switch(ec.value()) {
        case 10049:
            reason = "Bad ip address"; break;
        case 10051:
            reason = "Server unreachable"; break;
        case 10060:
            reason = "Connection timed out"; break;
        case 10061:
            reason = "Connection refused"; break;
        default:
            reason = QString::fromStdString(ec.message()) + "\nError code: " + QString::number(ec.value()); break;
    }
    ui->connection_status_label->setText("Could not connect to the server: " + reason);
}

void MainWindow::setupAppearance() {
    setStyleSheet("color: #f0f0f0; background-color: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 #272929, stop:1 #797687);} ");
}

void MainWindow::onServerMessageReceived(std::vector<std::string>& message_set) {
    auto const room_name{message_set[1]};
    auto const userlist{message_set[2]};
    auto const username{QString::fromStdString(message_set[3])};

    updateRoomName(room_name);
    updateUserlist(strToVectorUserlist(userlist));

    if (MessageTypeConvertions::strToMessageType(message_set[0]) == MessageType::USER_CONNECTED)
        ui->chatTextWidget->appendPlainText(username + " has connected to the server!");

    else if (MessageTypeConvertions::strToMessageType(message_set[0]) == MessageType::USER_LEFT
             && Utility::strToUsertype(message_set[4]) == Utility::Usertype::ADMIN) {
        onAdminLeave();
    }

    else
        ui->chatTextWidget->appendPlainText(username + " has left the server. ");
}

void MainWindow::onAdminLeave() {
    if (!owned) {
        auto message_box{new QMessageBox{this}};
        message_box->setText("Owner has left the server, the room got closed.");

        message_box->exec();

        connected = false;
        on_leaveLobbyButton_clicked();
    }

    else
        on_leaveLobbyButton_clicked();
}

void MainWindow::on_exitLobbyButton_clicked() {
    QApplication::quit();
}

void MainWindow::on_leaveLobbyButton_clicked() {
    if (connected) {
        client->disconnect();
        connected = false;
    }

    if (hosted) {
        server->stop();
        hosted = false;
    }

    clearWidgets();

    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::clearWidgets() {
    clearInputs();

    ui->userlistWidget->clear();
    ui->chatTextWidget->clear();
}

void MainWindow::clearInputs() {
    ui->username_le->clear();
    ui->ip_address_le->clear();
    ui->port_le->clear();
    ui->connection_status_label->clear();

    ui->bad_ip_address_label->clear();
    ui->room_name_le->clear();
    ui->host_username_le->clear();
    ui->ip_address_le_2->clear();
    ui->server_port_le->clear();
}

std::vector<std::string> MainWindow::strToVectorUserlist(std::string_view user_list_str) {
    std::vector<std::string> user_list;
    boost::split(user_list, user_list_str, boost::is_any_of(","));
    return user_list;
}

void MainWindow::updateRoomName(std::string_view room_name) {
    ui->roomname_label->setText(QString::fromStdString(room_name.data()));
}
