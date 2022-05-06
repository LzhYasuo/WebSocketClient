#include "SocketSendTask.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTimer>

SocketSendTask::SocketSendTask(QObject *parent)
{

}

SocketSendTask::~SocketSendTask()
{

}

void SocketSendTask::Close()
{
    _timer->stop();
    _isClose = true;
}

void SocketSendTask::WhileSend()
{
    _mux.lock();
    while(_sendStr.size() > 0)
    {
        if(_isClose)
        {
            _timer->deleteLater();
            _timer = nullptr;
            _mux.unlock();
            emit ThreadQuit();
            return;
        }
        QString str = _sendStr.takeFirst();
        if(!str.isEmpty())
        {
            emit SendJson(str);
        }
    }
    _mux.unlock();
}

void SocketSendTask::Run()
{
    _timer = new QTimer(this);
    this->connect(_timer,&QTimer::timeout,this,&SocketSendTask::WhileSend);
    _timer->start(40);
}

void SocketSendTask::PushBackJsonData(JsonData data)
{
    _mux.lock();
    _jsonData.push_back(data);
    _mux.unlock();
}

bool SocketSendTask::CreateJson()
{
    _mux.lock();
    QJsonObject json;
    for(auto& i : _jsonData)
    {
        if(i._idList.size() != i._valueList.size())
        {
            _mux.unlock();
            return false;
        }
        if(i._idListAry.size() != i._valueListAry.size())
        {
            _mux.unlock();
            return false;
        }
        switch (i._type)
        {
            case JSONOBJ:
            {
                json.insert(i._id,i._value);
                break;
            }
            case JSONARYOBJ:
            {
                QJsonArray array;
                QJsonObject Object;
                for(int index = 0; index < i._idList.size(); index++)
                {
                    Object.insert(i._idList.at(index),i._valueList.at(index));
                }
                array.append(Object);
                json.insert(i._name,QJsonValue(array));
                break;
            }
            case JSONARYOBJLIST:
            {
                QJsonArray array;
                for(int aryindex = 0; aryindex < i._idListAry.size(); aryindex++)
                {
                    QJsonObject Object;
                    for(int index = 0; index < i._idListAry.at(aryindex).size(); index++)
                    {
                        Object.insert(i._idListAry.at(aryindex).at(index),i._valueListAry.at(aryindex).at(index));
                    }
                    array.append(Object);
                }
                json.insert(i._name,QJsonValue(array));
            }
        }
    }
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);

    QString msg = QString(byteArray);
    _jsonData.clear();
    _sendStr.push_back(msg);
    _mux.unlock();
    return true;
}

JsonData &SocketSendTask::GetObj()
{
     static JsonData data;
     data._type = JSONOBJ;

     return data;
}

JsonData& SocketSendTask::GetBasicInformation()
{
    static JsonData data;
    data._name = "basicInfomation";
    data._idList = QStringList() << "functionName" << "sessionID";
    data._type = JSONARYOBJ;
    return data;
}

JsonData &SocketSendTask::GetMachineInformation()
{
    static JsonData data;
    data._name = "machineInformation";
    data._idList = QStringList() << "lineNo" << "machineNo";
    data._type = JSONARYOBJ;
    return data;
}

JsonData &SocketSendTask::GetNozzleReport(NozzleReport Model, int size)
{
    static JsonData data;
    data._name = "nozzleReport";
    data._type = JSONARYOBJLIST;
    switch (Model)
    {
        case ExamineModel:
        {
            for(int i = 0; i < size; i++)
            {
                data._idListAry.push_back(QStringList() << "unitSerialNo" << "nozzleName" << "result" << "vacuum");
            }
            break;
        }
        case NozPosModel:
        {
            for(int i = 0; i < size; i++)
            {
                data._idListAry.push_back(QStringList() << "unitSerialNo" << "nozzleName" << "table" << "stockNo");
            }
        }
    }
    return data;
}

void SocketSendTask::GetObj(JsonData& data)
{
    data._type = JSONOBJ;
}

void SocketSendTask::GetBasicInformation(JsonData& data)
{
    data._name = "basicInfomation";
    data._idList = QStringList() << "functionName" << "sessionID";
    data._type = JSONARYOBJ;
}

void SocketSendTask::GetMachineInformation(JsonData& data)
{
    data._name = "machineInformation";
    data._idList = QStringList() << "lineNo" << "machineNo";
    data._type = JSONARYOBJ;
}

void SocketSendTask::GetNozzleReport(JsonData& data, NozzleReport Model,int size)
{
    data._name = "nozzleReport";
    data._type = JSONARYOBJLIST;
    switch (Model)
    {
        case ExamineModel:
        {
            for(int i = 0; i < size; i++)
            {
                data._idListAry.push_back(QStringList() << "unitSerialNo" << "nozzleName" << "result" << "vacuum");
            }
            break;
        }
        case NozPosModel:
        {
            for(int i = 0; i < size; i++)
            {
                data._idListAry.push_back(QStringList() << "unitSerialNo" << "nozzleName" << "table" << "stockNo");
            }
        }
    }
}
