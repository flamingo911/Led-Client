#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtNetwork>
#include <QCloseEvent>
#include <QtWidgets>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mSocket(QSharedPointer<QTcpSocket>::create())
{
    ui->setupUi(this);
    InitHostComboBox();

    ui->portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    connect(mSocket.data(), &QIODevice::readyRead, this, &MainWindow::onReadyRead);
    connect(mSocket.data(), QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MainWindow::onTcpError);
}

MainWindow::~MainWindow()
{    
    delete ui;
}

void MainWindow::InitHostComboBox()
{
    ui->hostComboBox->addItem("localhost");
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i).isLoopback())
            ui->hostComboBox->addItem(ipAddressesList.at(i).toString());
    }
}

void MainWindow::on_requestPushButton_clicked()
{
    mSocket->abort();
    mSocket->connectToHost(ui->hostComboBox->currentText(), ui->portLineEdit->text().toUShort());
    const bool is_connected = mSocket->waitForConnected(3000);

    if(is_connected)
    {
        auto req = ui->requestLineEdit->text().toLatin1();
        mSocket->write(req);
    }
}

void MainWindow::onReadyRead()
{
    QByteArray response = mSocket->readAll();
    ui->responseLineEdit->setText(response);
}

void MainWindow::onTcpError(QAbstractSocket::SocketError socket_error)
{
    switch (socket_error) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, "Ошибка", "Хост не найден. Проверьте настройки порта и адреса.");
        break;

    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, "Ошибка",
                                 "Соединение было отклонено сервером."
                                 "Убедитесь, что сервер работает и проверьте настройки.");
        break;

    default:
        QMessageBox::information(this, tr("Ошибка"), tr("Была получена ошибка: %1.").arg(mSocket->errorString()));
    }
}
