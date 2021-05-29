#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Utility.h"
#include "KeyboardResponsiveTextEdit.h"

#include <QString>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->inputTextEdit->setPlaceholderText("Type here");
    ui->plainTextEdit->setReadOnly(true);
    Utility::set_button_icon(ui->emojiButton, ":/img/img/emoji.png", 1.5);
    Utility::set_button_icon(ui->attachmentsButton, ":/img/img/clip.png", 1.5);
    Utility::set_button_icon(ui->sendButton, ":/img/img/arrow.png", 1.1);

    client = std::make_unique<Client>();
    connect(client.get(), &Client::received, this, &MainWindow::onReceived, Qt::BlockingQueuedConnection);
    connect(client.get(), &Client::errorOccured, this, &MainWindow::onErrorOccured, Qt::BlockingQueuedConnection);
    connect(client.get(), &Client::connected, this, &MainWindow::onConnected, Qt::BlockingQueuedConnection);
    connect(client.get(), &Client::badConnect, this, &MainWindow::onBadConnect, Qt::BlockingQueuedConnection);

    connect(ui->inputTextEdit, &KeyboardResponsiveTextEdit::enterPressed, this, &MainWindow::onEnterPressed);
}

MainWindow::~MainWindow() {
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

    system::error_code address_ec;
    auto const ip_address{ boost::asio::ip::address::from_string(ip_address_raw, address_ec) };
    if (!address_ec)
          client->connect(ip_address, port);
}

void MainWindow::on_exitButton_2_clicked() {
    QApplication::exit();
}

void MainWindow::on_backButton_3_clicked() {
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_startButton_clicked() {
    //auto const port{ui->server_port_le};
    //auto const username{ui->username_le->text().toStdString()};
    //auto server{std::make_shared<Server>(port)};
    //server->start();
}

void MainWindow::on_exitButton_3_clicked() {
    QApplication::exit();
}

void MainWindow::on_sendButton_clicked() {
    sendMessage();
}

void MainWindow::onReceived(std::string_view message) {
    ui->plainTextEdit->appendPlainText(QString::fromStdString(message.data()));
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
    auto const message{ui->inputTextEdit->toPlainText().toStdString() + '\n'};
    client->write(message);
    ui->inputTextEdit->clear();
}

void MainWindow::onConnected() {
    client->setClientUsername(ui->username_le->text().toStdString());
    ui->username_label->setText(ui->username_le->text());
    ui->stackedWidget->setCurrentIndex(1);
    ui->plainTextEdit->setPlainText("Connected to the server");
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
