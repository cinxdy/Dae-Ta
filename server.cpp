#include "server.h"
#include <QTcpSocket>
#include <QRegExp>
#include <QTextCodec>
#include <QString>

server::server(QObject* parent): QTcpServer(parent)
{
    message=new Message();
    printf("server created\n");

    bool success=listen(QHostAddress::Any,8520);

    if(!success)
    {
        printf("Could not listen on port 8520.\n");
        return ;
    }
    printf("Ready\n");
}

void server::incomingConnection(int socketfd){

    client=new QTcpSocket(this);
    client->setSocketDescriptor(socketfd); //소켓드스크립터 설정

    printf("New Client from:%s\n",client->peerAddress().toString().toLocal8Bit().data());
    client->write("hello client\n");

    connect(client,SIGNAL(readyRead()),this,SLOT(readyRead()));
//    connect(client,SIGNAL(disconnected()),this,SLOT(disconnected()));
}

void server::readyRead()
{
    QTcpSocket* guest=static_cast<QTcpSocket*>(sender()); //메시지가 들어온 클라이언트를 감지하는 함수 sender
    printf("available:%d\n",guest->canReadLine());

    while(guest->canReadLine()){
        QString line=QString::fromUtf8(guest->readLine());
        printf("Read Line:%s\n",line.toLocal8Bit().data());
    }
}

void server::sendMessage(){
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out<<message;
    printf("write\n");
    QTextStream(stdout)<<message;
    client->write(dat);
}
