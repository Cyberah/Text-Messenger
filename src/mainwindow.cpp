#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Utility.h"
#include "KeyboardResponsiveTextEdit.h"

#include <QString>
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->inputTextEdit->setPlaceholderText("Type here");
    ui->plainTextEdit->setReadOnly(true);

    animatedLabelFactory = new AnimatedLabel{this};
    animatedLabelFactory->createAnimation(ui->animatedLabel);

    setupAppearance();

    Utility::set_button_icon(ui->emojiButton, ":/img/img/emoji.png", 1.5);
    Utility::set_button_icon(ui->attachmentsButton, ":/img/img/clip.png", 1.5);
    Utility::set_button_icon(ui->sendButton, ":/img/img/arrow.png", 1.1);

    client = std::make_unique<Client>();
    server = std::make_unique<Server>();

    connect(client.get(), &Client::messageReceived, this, &MainWindow::onMessageReceived, Qt::BlockingQueuedConnection);
    connect(client.get(), &Client::errorOccured, this, &MainWindow::onErrorOccured, Qt::BlockingQueuedConnection);
    connect(client.get(), &Client::connected, this, &MainWindow::onConnected, Qt::BlockingQueuedConnection);
    connect(client.get(), &Client::badConnect, this, &MainWindow::onBadConnect, Qt::BlockingQueuedConnection);
    connect(client.get(), &Client::received_info, this, &MainWindow::onReceivedInfo, Qt::BlockingQueuedConnection);
    connect(client.get(), &Client::serverMessageReceived, this, &MainWindow::onServerMessageReceived, Qt::BlockingQueuedConnection);

    connect(ui->inputTextEdit, &KeyboardResponsiveTextEdit::enterPressed, this, &MainWindow::onEnterPressed);
}

MainWindow::~MainWindow() {
    if (hosted)
        server->stop();

    if (connected)
        client->disconnect();

    delete ui;
}

void MainWindow::on_connectButton_clicked() {
    ui->stackedWidget_2->setCurrentIndex(1);
}

void MainWindow::on_hostButton_clicked() {
    ui->stackedWidget_2->setCurrentIndex(2);
}

void MainWindow::on_exitButton_clicked() {
    QApplication::exit();
}

void MainWindow::on_backButton_2_clicked() {
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_connectButton_2_clicked() {
    ui->connection_status_label->setText("Connecting to " + ui->ip_address_le->text() + "...");
    auto const ip_address_raw{ui->ip_address_le->text().toStdString()};

    auto const port{ui->port_le->text().toShort()};
    auto const username{ui->username_le->text().toStdString()};

    system::error_code bad_address;
    auto const ip_address{ boost::asio::ip::address::from_string(ip_address_raw, bad_address) };

    //client.reset(new Client);
    if (!bad_address)
        client->connect(ip_address, port, Utility::Usertype::USER);
}

void MainWindow::on_exitButton_2_clicked() {
    QApplication::exit();
}

void MainWindow::on_backButton_3_clicked() {
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_startButton_clicked() {
    auto const port{ui->server_port_le->text().toShort()};
    auto const address_raw{ui->ip_address_le_2->text().toStdString()};

    auto const username{ui->host_username_le->text().toStdString()};
    auto const roomname{ui->room_name_le->text().toStdString()};

    system::error_code bad_address;
    auto const address{asio::ip::address::from_string(address_raw, bad_address)};

    //server.reset(new Server);
    //client.reset(new Client);
    if (!bad_address) {
        try {
            server->start(address, port);
            server->setRoomname(roomname);

            hosted = true;
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
    QApplication::exit();
}

void MainWindow::on_sendButton_clicked() {
    sendMessage();
}

void MainWindow::onMessageReceived(std::string_view message) {
    auto const[complete_message, user_list]{Utility::process_message(message)};

    updateUserlist(user_list);
    ui->plainTextEdit->appendPlainText(QString::fromStdString(complete_message));
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
        auto const message{ui->inputTextEdit->toPlainText().toStdString() + '\n'};

        client->write(message);
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

void MainWindow::onReceivedInfo(std::pair<std::string, std::vector<std::string>> const& info) {
    ui->roomname_label->setText(QString::fromStdString(info.first));
    updateUserlist(info.second);

    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::setupAppearance() {
    setStyleSheet("color: #f0f0f0; background-color: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 #272929, stop:1 #797687);} ");
}

void MainWindow::onServerMessageReceived(std::string_view str) {
    auto const username{QString::fromStdString(str.data())};
    ui->plainTextEdit->appendPlainText(username + " has connected to the server!");
}

void MainWindow::on_exitLobbyButton_clicked() {
    QApplication::exit();
}

void MainWindow::on_leaveLobbyButton_clicked() {
    if (hosted) {
        server->stop();
        hosted = false;
    }

    if (connected) {
        client->disconnect();
        connected = false;
    }

    clearAllInputs();

    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::clearAllInputs() {
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
