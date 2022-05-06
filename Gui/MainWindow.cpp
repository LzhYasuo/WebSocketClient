#include "MainWindow.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QThread>
#include <QDebug>

#include "../Task/SingletonSocket.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << QThread::currentThreadId();

    SingletionSocket::Get()->NetWorkInit("ws://192.168.0.120:11098");
    QHBoxLayout* mLayout = new QHBoxLayout();
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0,0,0,0);

    QPushButton* btn1 = new QPushButton();
    QPushButton* btn2 = new QPushButton();
    QPushButton* btn3 = new QPushButton();
    mLayout->addWidget(btn1);
    mLayout->addWidget(btn2);
    mLayout->addWidget(btn3);

    this->connect(btn1,&QPushButton::clicked,this,[=](){
        QList<QStringList> valueListAry1;
        QStringList list1 = QStringList() << "NozzlePosReport" << "17";
        QStringList list2 = QStringList() << "F5-NozzleChecker01" << "01";
        valueListAry1.push_back(list1);
        valueListAry1.push_back(list2);
        QList<QStringList> valueListAry2;
        valueListAry2.push_back(QStringList() << "160564" << "230S" << "1" << "101");
        valueListAry2.push_back(QStringList() << "160564" << "230S" << "1" << "101");
        valueListAry2.push_back(QStringList() << "160564" << "230S" << "1" << "101");

        SingletionSocket::Get()->PushBackData(valueListAry1,CheckdataModel,valueListAry2);
    });

    this->setLayout(mLayout);
}

MainWindow::~MainWindow()
{

}

void MainWindow::Init()
{
}
