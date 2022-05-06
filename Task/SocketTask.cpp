#include "SocketTask.h"
#include <QWebSocket>
#include <QDebug>
#include <QThread>

SocketTask::SocketTask(QObject* parent)
    :QObject (parent)
{

}

SocketTask::~SocketTask()
{

}

void SocketTask::SetUrl(QString url)
{
    _url = url;
}

void SocketTask::SocketConnect()
{
    qDebug() << QThread::currentThreadId();
   _webSocket = new QWebSocket();
   _webSocket->open(_url);
   this->connect(_webSocket,&QWebSocket::connected,this,[=](){
//       _timer = new QTimer();
//       qDebug() << "Connect Success!";
//       this->connect(_timer,&QTimer::timeout,this,[=](){
//           QAbstractSocket::SocketState state = _webSocket->state();
//           if(state == QAbstractSocket::UnconnectedState)
//           {
//               emit SockDisConnected();
//           }
//       });
//       _timer->start(3000);
   });
   this->connect(_webSocket,&QWebSocket::disconnected,this,[=](){
       _webSocket->close();
   });
   this->connect(_webSocket,&QWebSocket::textMessageReceived,this,[=](QString msg){
        emit SendJson(msg);
   });
}

void SocketTask::SendMsg(QString msg)
{
//    int datelen = msg.length();
//    if(datelen > 0)
//    {
        _webSocket->sendTextMessage(msg);
//    }
}

void SocketTask::Close()
{
    _timer->deleteLater();
    _timer = nullptr;
    _webSocket->close();
    emit ThreadQuit();
}

