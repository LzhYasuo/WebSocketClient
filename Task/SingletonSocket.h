#ifndef _SINGLETION_SOCKET_H_
#define _SINGLETION_SOCKET_H_

#include <QObject>

enum CreateJsonModel
{
    CheckdataModel,             //检查数据格式 BasicInformation machineInformation nozzleReport
    NozzlepositionModel,        //吸嘴位置格式 BasicInformation machineInformation nozzleReport
    ConnectionpacketsModel,     //连接数据包嗯 BasicInformation clientID clientType
    ReturnpacketsModel,         //返回数据包嗯 BasicInformation clientType ipAdd lineNo
};

class SocketSendTask;
class SingletionSocket : public QObject
{
    Q_OBJECT
public:
    static SingletionSocket* Get()
    {
        static SingletionSocket cx;
        return &cx;
    }
    bool PushBackData(QList<QStringList>,CreateJsonModel,QList<QStringList> lotList = QList<QStringList>());
    void NetWorkInit(QString url);
    void Close();
signals:
    void AllClose();

private:
    SingletionSocket(QObject* parent = nullptr);
    ~SingletionSocket();
private:
    bool isCreate = false;
    SocketSendTask* _otherSend = nullptr;
};


#endif //_SINGLETION_SOCKET_H_
