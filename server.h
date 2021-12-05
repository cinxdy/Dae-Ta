#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include "home.h"

class Message{
 int stateLocation;

};


class server: public QTcpServer
{
    Q_OBJECT
public:
    server(QObject* parent=0);

private:
    Message *message;

protected:
    void incomingConnection(int socketfd);

private slots:
    void readyRead();

public slots:
    void sendMessage();

private:
    QTcpSocket *client;
};

#endif // SOCKET_H