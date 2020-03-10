#pragma once

#ifndef LIGHTCLIENT_H
#define LIGHTCLIENT_H

#include <QObject>
#include <QTcpSocket>

//QT_BEGIN_NAMESPACE
//class QTcpSocket;
//QT_END_NAMESPACE

class LightClient : public QObject
{
    Q_OBJECT
public:
    explicit LightClient(QObject *parent = nullptr);
    void SendCommand(const QString& host, const uint16_t port, const QString msg) const;

private slots:
    void onTcpError(QAbstractSocket::SocketError socketError);
    void onReadyRead();

private:
    QSharedPointer<QTcpSocket> mSocket;
};

#endif // LIGHTCLIENT_H
