#include "SocketAcceptTask.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

SocketAcceptTask::SocketAcceptTask(QObject *parent)
{

}

SocketAcceptTask::~SocketAcceptTask()
{

}

void SocketAcceptTask::Run()
{
    _timer = new QTimer(this);
    this->connect(_timer,&QTimer::timeout,this,&SocketAcceptTask::WhileAccept);
    _timer->start(40);
}

void SocketAcceptTask::WhileAccept()
{
    _mux.lock();
    while(_acceptList.size() > 0)
    {
        if(_isClose)
        {
            _timer->deleteLater();
            _timer = nullptr;
            _mux.unlock();
            emit ThreadQuit();
            return;
        }
        QString str = _acceptList.takeFirst();
        if(!str.isEmpty())
        {
            qDebug() << str;
            QStringList keyList;
            QStringList valueList;
            ParseJson(str,keyList,valueList);

            qDebug() << keyList;
            qDebug() << valueList;
        }
    }
    _mux.unlock();
}

void SocketAcceptTask::ParseJson(QString str,QStringList& keyList,QStringList& valueList)
{
    const QByteArray jsonStr = str.toUtf8();
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(jsonStr, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        // 解析未发生错误
        if (doucment.isObject())
        {
             QJsonObject object = doucment.object();  // 转化为对象
            // JSON 文档为对象
            if (object.contains("basicInfomation"))
            {
                QJsonValue basiValue = object.value("basicInfomation");
                if(basiValue.isArray())
                {
                    QJsonArray array = basiValue.toArray();
                    const int nSize = array.size();
                    for (int i = 0; i < nSize; ++i)
                    {
                        QJsonValue aryValue = array.at(i);
                        if (aryValue.isObject())
                        {
                            QJsonObject aryObj = aryValue.toObject();
                            auto it = aryObj.begin();
                            for(; it != aryObj.end(); it++)
                            {
                                if(it.key() == "functionName" || it.key() == "sessionID")
                                {
                                    keyList.push_back(it.key());
                                    valueList.push_back(it.value().toString());
                                }
                            }
                        }
                    }
                }
            }
            if(object.contains("machineInformation"))
            {
                QJsonValue machieValue = object.value("machineInformation");
                if(machieValue.isArray())
                {
                    if(machieValue.isArray())
                    {
                        QJsonArray array = machieValue.toArray();
                        const int nSize = array.size();
                        for (int i = 0; i < nSize; ++i)
                        {
                            QJsonValue aryValue = array.at(i);
                            if (aryValue.isObject())
                            {
                                QJsonObject aryObj = aryValue.toObject();
                                auto it = aryObj.begin();
                                for(; it != aryObj.end(); it++)
                                {
                                    if(it.key() == "lineNo" || it.key() == "machineNo")
                                    {
                                        keyList.push_back(it.key());
                                        valueList.push_back(it.value().toString());
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(object.contains("nozzleReport"))
            {
                QJsonValue nozzleValue = object.value("nozzleReport");
                if(nozzleValue.isArray())
                {
                    if(nozzleValue.isArray())
                    {
                        QJsonArray array = nozzleValue.toArray();
                        const int nSize = array.size();
                        for (int i = 0; i < nSize; ++i)
                        {
                            QJsonValue aryValue = array.at(i);
                            if (aryValue.isObject())
                            {
                                QJsonObject aryObj = aryValue.toObject();
                                auto it = aryObj.begin();
                                for(; it != aryObj.end(); it++)
                                {
                                    if(it.key() == "unitSerialNo" || it.key() == "nozzleName" || it.key() == "result" || it.key() == "vacuum"
                                     ||it.key() == "table" || it.key() == "stockNo")
                                    {
                                        keyList.push_back(it.key());
                                        valueList.push_back(it.value().toString());
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(object.contains("clientType"))
            {
                QJsonValue clientTypeValue = object.value("clientType");
                if(clientTypeValue.isString())
                {
                    QString strClientType = clientTypeValue.toString();
                    keyList.push_back("clientType");
                    valueList.push_back(strClientType);
                }
            }
            if(object.contains("clientID"))
            {
                QJsonValue clientIDValue = object.value("clientID");
                if(clientIDValue.isString())
                {
                    QString strClientID = clientIDValue.toString();
                    keyList.push_back("clientID");
                    valueList.push_back(strClientID);
                }
            }
            if(object.contains("ipAdd"))
            {
                QJsonValue ipAddValue = object.value("ipAdd");
                if(ipAddValue.isString())
                {
                    QString strIpAdd = ipAddValue.toString();
                    keyList.push_back("ipAdd");
                    valueList.push_back(strIpAdd);
                }
            }
            if(object.contains("lineNo"))
            {
                QJsonValue lineNoValue = object.value("lineNo");
                if(lineNoValue.isString())
                {
                    QString strLineNo = lineNoValue.toString();
                    keyList.push_back("lineNo");
                    valueList.push_back(strLineNo);
                }
            }
        }
    }
}

void SocketAcceptTask::Close()
{
    _timer->stop();
    _isClose = true;
}

void SocketAcceptTask::PushBackData(QString msg)
{
    _mux.lock();
    _acceptList.push_back(msg);
    _mux.unlock();
}
