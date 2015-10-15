#include "client.h"

client::client()
{
    this->socket = new QTcpSocket();
    connect(this->socket, SIGNAL(readyRead()),this,SLOT(readData()));
    this->socket->connectToHost(QHostAddress::LocalHost,4444);
}

void client::readData()
{
    QString s;
    while(socket->bytesAvailable() > 0) {
        QString result = socket->readLine();
        s += result;
    }
    emit change();
}
