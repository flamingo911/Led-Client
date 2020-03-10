#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "lightclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void InitHostComboBox();


private slots:
    void on_requestPushButton_clicked();
    void onTcpError(QAbstractSocket::SocketError socket_error);
    void onReadyRead();

private:
    Ui::MainWindow *ui;
    QSharedPointer<QTcpSocket> mSocket;
};
#endif // MAINWINDOW_H
