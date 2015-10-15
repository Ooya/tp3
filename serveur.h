#ifndef SERVEUR_H
#define SERVEUR_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QVector>
#include <QString>
#include <algorithm>

class serveur : public QObject
{
    Q_OBJECT
    public:
        serveur();
        void run();

    signals :
        //void changeSeason(int season);

    public slots:
        void emitSeason();

    private slots :
        void newClient();

    private:
        QTcpServer* server;

        QTimer* time;

        QVector<QTcpSocket* > clients;

    };

#endif // SERVEUR_H
