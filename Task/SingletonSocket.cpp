#include "SingletonSocket.h"
#include "SocketAcceptTask.h"
#include "SocketSendTask.h"
#include "SocketTask.h"
#include <QThread>

SingletionSocket::SingletionSocket(QObject *parent)
    :QObject (parent)
{

}

SingletionSocket::~SingletionSocket()
{

}

void SingletionSocket::NetWorkInit(QString url)
{
    if(isCreate)
        return;
    //AcceptThread
    QThread* acceptThread = new QThread();
    SocketAcceptTask* acceptObj = new SocketAcceptTask();
    acceptObj->moveToThread(acceptThread);

    this->connect(this,&SingletionSocket::AllClose,acceptObj,&SocketAcceptTask::Close);
    this->connect(acceptThread,&QThread::started,acceptObj,&SocketAcceptTask::Run);
    this->connect(acceptObj,&SocketAcceptTask::ThreadQuit,acceptThread,&QThread::quit);
    this->connect(acceptThread,&QThread::finished,acceptObj,&SocketAcceptTask::deleteLater);
    this->connect(acceptThread,&QThread::finished,acceptThread,&QThread::deleteLater);
    acceptThread->start();

    //SendThread
    QThread* sendThread = new QThread();
    SocketSendTask* sendObj = new SocketSendTask();
    sendObj->moveToThread(sendThread);
    _otherSend = sendObj;

    this->connect(this,&SingletionSocket::AllClose,sendObj,&SocketSendTask::Close);
    this->connect(sendThread,&QThread::started,sendObj,&SocketSendTask::Run);
    this->connect(sendObj,&SocketSendTask::ThreadQuit,sendThread,&QThread::quit);
    this->connect(sendThread,&QThread::finished,sendObj,&SocketSendTask::deleteLater);
    this->connect(sendThread,&QThread::finished,sendThread,&QThread::deleteLater);
    sendThread->start();

    //WebSocket
    QThread* SocketThread = new QThread();
    SocketTask* socketObj = new SocketTask();
    socketObj->moveToThread(SocketThread);
    socketObj->SetUrl(url);
//    socketObj->SocketConnect();

    this->connect(SocketThread,&QThread::started,socketObj,&SocketTask::SocketConnect);
    this->connect(this,&SingletionSocket::AllClose,socketObj,&SocketTask::Close);
    this->connect(socketObj,&SocketTask::ThreadQuit,SocketThread,&QThread::quit);
    this->connect(SocketThread,&QThread::finished,socketObj,&SocketTask::deleteLater);
    this->connect(SocketThread,&QThread::finished,SocketThread,&QThread::deleteLater);

    //WebSocket外部函数对接
    this->connect(sendObj,&SocketSendTask::SendJson,socketObj,&SocketTask::SendMsg,Qt::QueuedConnection);
    this->connect(socketObj,&SocketTask::SendJson,acceptObj,&SocketAcceptTask::PushBackData,Qt::QueuedConnection);
    SocketThread->start();

    isCreate = true;
}

bool SingletionSocket::PushBackData(QList<QStringList> list,CreateJsonModel Model,QList<QStringList> lotList)
{
    if(_otherSend == nullptr)
        return false;
    switch (Model)
    {
        case CheckdataModel:
        {
            if(list.size() != 2)
            {
                return false;
            }
            if(lotList.size() <= 0)
            {
                return false;
            }
            //BasicInformation
            JsonData basiData;
            QStringList basiValue = list.at(0);
            _otherSend->GetBasicInformation(basiData);
            basiData._valueList = basiValue;

            //machineInformation
            JsonData machieData;
            QStringList machineValue = list.at(1);
            _otherSend->GetMachineInformation(machieData);
            machieData._valueList = machineValue;

            //nozzleReport
            JsonData nozzleData;
            const int nozzleCount = lotList.size();
            _otherSend->GetNozzleReport(nozzleData,ExamineModel,nozzleCount);
            nozzleData._valueListAry = lotList;

            //生成Json
            _otherSend->PushBackJsonData(basiData);
            _otherSend->PushBackJsonData(machieData);
            _otherSend->PushBackJsonData(nozzleData);
            bool isCreate = _otherSend->CreateJson();
            if(!isCreate)
            {
                return false;
            }
            break;
        }
        case NozzlepositionModel:
        {
            if(list.size() != 2)
            {
                return false;
            }
            if(lotList.size() <= 0)
            {
                return false;
            }
            //BasicInformation
            JsonData basiData;
            QStringList basiValue = list.at(0);
            _otherSend->GetBasicInformation(basiData);
            basiData._valueList = basiValue;

            //machineInformation
            JsonData machieData;
            QStringList machineValue = list.at(1);
            _otherSend->GetMachineInformation(machieData);
            machieData._valueList = machineValue;

            //nozzleReport
            JsonData nozzleData;
            const int nozzleCount = lotList.size();
            _otherSend->GetNozzleReport(nozzleData,NozPosModel,nozzleCount);
            nozzleData._valueListAry = lotList;

            //生成Json
            _otherSend->PushBackJsonData(basiData);
            _otherSend->PushBackJsonData(machieData);
            _otherSend->PushBackJsonData(nozzleData);
            bool isCreate = _otherSend->CreateJson();
            if(!isCreate)
            {
                return false;
            }
            break;
        }
        case ConnectionpacketsModel:
        {
            if(list.size() != 3)
            {
                return false;
            }
            //BasicInformation
            JsonData basiData;
            QStringList basiValue = list.at(0);
            _otherSend->GetBasicInformation(basiData);
            basiData._valueList = basiValue;

            //Type
            QStringList clientTypeList = list.at(1);
            if(clientTypeList.size() != 2)
            {
                return false;
            }
            JsonData cTypeData;
            _otherSend->GetObj(cTypeData);
            cTypeData._id = clientTypeList.at(0);
            cTypeData._value = clientTypeList.at(1);

            //ID
            QStringList clientIDList = list.at(2);
            if(clientIDList.size() != 2)
            {
                return false;
            }
            JsonData cIDData;
            _otherSend->GetObj(cIDData);
            cIDData._id = clientIDList.at(0);
            cIDData._value = clientIDList.at(1);

            //生成Json
            _otherSend->PushBackJsonData(basiData);
            _otherSend->PushBackJsonData(cTypeData);
            _otherSend->PushBackJsonData(cIDData);
            bool isCreate = _otherSend->CreateJson();
            if(!isCreate)
            {
                return false;
            }
            break;
        }
        case ReturnpacketsModel:
        {
            if(list.size() != 4)
            {
                return false;
            }
            //BasicInformation
            JsonData basiData;
            QStringList basiValue = list.at(0);
            _otherSend->GetBasicInformation(basiData);
            basiData._valueList = basiValue;

            //Type
            QStringList clientTypeList = list.at(1);
            if(clientTypeList.size() != 2)
            {
                return false;
            }
            JsonData cTypeData;
            _otherSend->GetObj(cTypeData);
            cTypeData._id = clientTypeList.at(0);
            cTypeData._value = clientTypeList.at(1);

            //IP
            QStringList ipAddList = list.at(2);
            if(ipAddList.size() != 2)
            {
                return false;
            }
            JsonData ipAddData;
            _otherSend->GetObj(ipAddData);
            ipAddData._id = ipAddList.at(0);
            ipAddData._value = ipAddList.at(1);

            //LineNo
            QStringList LineNoList = list.at(2);
            if(LineNoList.size() != 2)
            {
                return false;
            }
            JsonData lineNoData;
            _otherSend->GetObj(lineNoData);
            lineNoData._id = LineNoList.at(0);
            lineNoData._value = LineNoList.at(1);

            //生成Json
            _otherSend->PushBackJsonData(basiData);
            _otherSend->PushBackJsonData(cTypeData);
            _otherSend->PushBackJsonData(ipAddData);
            _otherSend->PushBackJsonData(lineNoData);
            bool isCreate = _otherSend->CreateJson();
            if(!isCreate)
            {
                return false;
            }

            break;
        }
    }
    return true;
}

void SingletionSocket::Close()
{
    _otherSend = nullptr;
    isCreate = false;
    emit AllClose();
}
