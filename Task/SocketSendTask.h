#ifndef _SOCKET_SEND_TASK_H_
#define _SOCKET_SEND_TASK_H_

#include <QObject>
#include <QMutex>
#include <QTimer>

enum JsonType
{
    JSONOBJ,        //object
    JSONARYOBJ,     //array[obj]
    JSONARYOBJLIST, //array[obj,obj,obj...]
};

enum NozzleReport
{
    ExamineModel,       //检测模式
    NozPosModel,        //吸嘴位置模式
};

struct JsonData
{
    QString _name;
    JsonType _type;
    QString _id;
    QString _value;
    QStringList _idList;
    QStringList _valueList;
    QList<QStringList> _idListAry;
    QList<QStringList> _valueListAry;
};

class SocketSendTask : public QObject
{
    Q_OBJECT
public:

    SocketSendTask(QObject* parent = nullptr);
    ~SocketSendTask();

    void Close();
    void WhileSend();
    void Run();
    void PushBackJsonData(JsonData);
    bool CreateJson();

    JsonData& GetObj();
    JsonData& GetBasicInformation();
    JsonData& GetMachineInformation();
    JsonData& GetNozzleReport(NozzleReport,int);
    void GetObj(JsonData&);
    void GetBasicInformation(JsonData&);
    void GetMachineInformation(JsonData&);
    void GetNozzleReport(JsonData&,NozzleReport,int size);
signals:
    void SendJson(QString);
    void ThreadQuit();
private:

private:
    bool _isClose = false;
    QStringList _sendStr;
    QList<JsonData> _jsonData;
    QTimer* _timer;
    QMutex _mux;
};

#endif //_SOCKET_SEND_TASK_H_
