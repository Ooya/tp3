#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QHostAddress>

class client : public QObject
{
    Q_OBJECT
    public:
        client();
        public slots:
        void readData();

    private:
        QTcpSocket* socket;

    signals:
        void change();
};

#endif // CLIENT_H
