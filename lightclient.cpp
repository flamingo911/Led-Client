#include "lightclient.h"
#include <QtNetwork>
#include <QtWidgets>

LightClient::LightClient(QObject *parent) : QObject(parent), mSocket(new QTcpSocket)
{
    connect(mSocket.get(), &QIODevice::readyRead, this, &LightClient::onReadyRead);
    connect(mSocket.get(), QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &LightClient::onTcpError);
}

void LightClient::SendCommand(const QString& host, const uint16_t port, const QString msg) const
{
    mSocket->abort();
    mSocket->connectToHost(host, port);
    mSocket->write(msg.toLatin1());
}

void LightClient::onReadyRead()
{
    if(mSocket && mSocket->isOpen() && mSocket->isValid() && mSocket->isReadable())
    {
        return;
    }
}

void LightClient::onTcpError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(nullptr, tr("Fortune Client"), tr("The host was not found. Please check the host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(nullptr, tr("Fortune Client"), tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(nullptr, tr("Fortune Client"), tr("The following error occurred: %1.").
                                 arg(mSocket->errorString()));
    }
}
