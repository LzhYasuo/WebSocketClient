#ifndef _SOCKET_TASK_H_
#define _SOCKET_TASK_H_

#include <QObject>
#include <QTimer>

class QWebSocket;
class SocketTask : public QObject
{
    Q_OBJECT
public:
    SocketTask(QObject* parent = nullptr);
    ~SocketTask();

    void SetUrl(QString url);
    //连接
    void SocketConnect();
    //心跳
    void WhileState();
    //关闭
    void Close();
public slots:
    //发送
    void SendMsg(QString);
signals:
    void SendJson(QString);
    void SockDisConnected();
    void ThreadQuit();
private:
    QWebSocket* _webSocket = nullptr;
    QString _url;
    QTimer* _timer;
};

#endif //_SOCKET_TASK_H_
