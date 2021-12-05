#ifndef SOCKET_H
#define SOCKET_H
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>

class socket
{
public:
    socket();

private:
    QTcpSocket *tcpSocket; // socket
    quint16 nextBlockSize;

private slots:
    void connectToServer();
    void onConnectServer();
    void sendRequest();
    void readMessage();
    void connectionClosedByServer();
    void error();
};

#endif // SOCKET_H
