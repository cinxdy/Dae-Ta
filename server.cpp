#include "server.h"
#include <QTcpSocket>
#include <QRegExp>
#include <QTextCodec>
#include <QString>

server::server(QObject* parent): QTcpServer(parent)
{
    printf("server created\n");
    bool success=listen(QHostAddress::Any,8520);

    if(!success)
    {
        printf("Could not listen on port 8520.\n");
        return ;
    }
    printf("Ready\n");

    client=new QTcpSocket(this);
}

void server::incomingConnection(int socketfd){

    client->setSocketDescriptor(socketfd); //소켓드스크립터 설정

    printf("New Client from:%s\n",client->peerAddress().toString().toLocal8Bit().data());
    client->write("hello client\n");

    emit connected();

    connect(client,SIGNAL(readyRead()),this,SLOT(readyRead()));
//    connect(client,SIGNAL(disconnected()),this,SLOT(disconnected()));
}

void server::readyRead()
{
    QByteArray byte = client->readAll();
    QDataStream read(&byte, QIODevice::ReadOnly);
    read.readRawData(reinterpret_cast<char *>(&m), sizeof(m));
    QTextStream(stdout)<<"Got message::velocity:"<<m.velocity<<endl;
    emit changeVSignal(m.velocity);
}

void server::sendMessage(){
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    // out<<message;
    out.writeRawData(reinterpret_cast<const char*>(&m),sizeof(m));

    printf("write\n");
//    QTextStream(stdout)<<dat;
    
    client->write(dat);
}
