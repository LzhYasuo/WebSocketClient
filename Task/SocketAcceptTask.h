#ifndef _SOCKET_ACCEPT_TASK_H_
#define _SOCKET_ACCEPT_TASK_H_

#include <QObject>
#include <QMutex>
#include <QTimer>

class SocketAcceptTask : public QObject
{
    Q_OBJECT
public:
    SocketAcceptTask(QObject* parent = nullptr);
    ~SocketAcceptTask();

    void Run();
    void WhileAccept();
    void ParseJson(QString,QStringList&,QStringList&);
    void Close();
    void PushBackData(QString msg);
signals:
    void ThreadQuit();
private:
    QMutex _mux;
    QStringList _acceptList;
    bool _isClose = false;
    QTimer* _timer;
};

#endif //_SOCKET_ACCEPT_TASK_H_
