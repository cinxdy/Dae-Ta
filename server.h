#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include "message.h"

class server: public QTcpServer
{
    Q_OBJECT
public:
    server(QObject* parent=0);

//private:
    Message *message;

protected:
    void incomingConnection(int socketfd);

private slots:
    void readyRead();

public slots:
    void sendMessage();

signals:
    void changeVSignal(int);
    void connected();

private:
    QTcpSocket *client;

};

#endif // SOCKET_H
