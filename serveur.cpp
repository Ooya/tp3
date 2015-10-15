#include "serveur.h"

#include <QTimer>

serveur::serveur()
{
    this->server = new QTcpServer();

    if (!server->listen(QHostAddress::LocalHost, 4444)) {
        qDebug() << "impossible de lancer le serveur";
        return;
    }
    QString ipAddress;
    ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    qDebug() << "address = " << server->serverAddress() << "\nport = " << server->serverPort();
    connect(server, SIGNAL(newConnection()), this, SLOT(newClient()));

    this->time = new QTimer();
    time->start(10000);
    connect(time,SIGNAL(timeout()),this,SLOT(emitSeason()));
}

void serveur::run(){

}

void serveur::emitSeason()
{
    QString string = "changement";
    qDebug()<<"Serveur : changement de saison";
    std::for_each(this->clients.begin(), this->clients.end(), [string](QTcpSocket *s){
       s->write(string.toUtf8());
       s->flush();
    });
}

void serveur::newClient()
{
    QTcpSocket *clientConnection = server->nextPendingConnection();
    clients.push_back(clientConnection);
    qDebug()<<"Serveur : nouvelle conexion : "<<clientConnection->peerPort();
}
