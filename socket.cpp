#include "socket.h"

socket::socket()
{
    tcpSocket = new QTcpSocket;
}

void socket::connectToServer(){
    //서버 연결 요청
    tcpSocket->connectToHost(QHostAddress::LocalHost, 8010);
}

void socket::onConnectServer(){
    QTextStream(stdout)<<"connected"<<endl;
//    ui->textEdit->setText(tr("connect Complete"));
}

void socket::sendRequest(){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    //전송될 메시지의 사이즈 입력
    //사이즈는 가변이므로 일단 0으로
    out<<quint16(0);
// 
//    전송될 메시지 읽음
    // out<<ui->lineEdit->text();

    //사이즈를 생성
    //스트림에 포인터를 맨 앞으로
    out.device()->seek(0);

    //사이즈 입력
    out<<quint16(block.size() - sizeof(quint16));

    //소켓으로 전송
    tcpSocket->write(block);
}

void socket::readMessage(){
    QDataStream in(tcpSocket);

    while(true){
         //nextBlcokSize 가 0 이면 아직 데이터를 못받은것
        if(nextBlockSize == 0){
            //수신된 데이터가 nextBlockSize 바이트보다 큰지 확인
            if(tcpSocket->bytesAvailable() < sizeof(quint16))
                ;
            else
                in>>nextBlockSize;
            continue;
        }
        //nextBlcokSize가 도착하면 사이즈만큼 데이터가 도착했는지 확인
       else if(tcpSocket->bytesAvailable() < nextBlockSize)
            continue;

        //데이터를 표시
       else if(tcpSocket->bytesAvailable() >= nextBlockSize){
            QString strBuf;
            in>>strBuf;

//            ui->textEdit->setText(strBuf);
            this->nextBlockSize = 0;

            break;
        }
    }
}
