#include "serial.h"
#include <qDebug.h>

serial::serial()
{
    //qDebug()<<"创建对象初始化线程为:"<<QThread::currentThreadId();
}


serial::~serial()
{
    if(QSerialPort::isOpen())
        QSerialPort::close();
}


QList<QString> serial::refreshSerialPort()
{
    QList<QString> tmp;
    QSerialPort TmpSerial;

    //搜索串口
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QString status;
        QString msg;
        if(!info.isBusy())
        {
            status = info.isBusy() ? "BUSY  " : "IDLE  ";
            msg = info.portName() +
                    "(" +
                    status + info.description() +
                    ")" +
                    " " + info.manufacturer();
            tmp.append(msg);
        }

    }

    //排序
    QList<QString> sorted;
    if(false == tmp.isEmpty()){
        QList<QString> tmp2;//用于存放COM0-COM9的条目
        QList<QString> tmp3;//用于存放COM10-COM99的条目
        for(int i = 0; i < tmp.size(); i++){
            //COM号在0-9
            if(tmp.at(i).indexOf("(") == 4){
                tmp2.append(tmp.at(i));
            }
            else if(tmp.at(i).indexOf("(") == 5){
                tmp3.append(tmp.at(i));
            }

        }
        tmp2.sort();
        tmp3.sort();
        sorted.append(tmp2);
        sorted.append(tmp3);
    }else{
        sorted = tmp;
    }

    return sorted;
}


bool serial::open(QString PortName, int BaudRate)
{
    QSerialPort::setPortName(PortName);
    QSerialPort::setBaudRate(BaudRate);
    QSerialPort::setDataBits(QSerialPort::Data8);
    QSerialPort::setParity(QSerialPort::NoParity);
    QSerialPort::setStopBits(QSerialPort::OneStop);
    QSerialPort::setFlowControl(QSerialPort::NoFlowControl);
    return QSerialPort::open(QSerialPort::ReadWrite);
}


int64_t serial::write(const QByteArray &data)
{
    int64_t tmp;

    tmp = QSerialPort::write(data);
    return tmp;
}


QByteArray serial::readAll()
{
    QByteArray tmp;

    tmp = QSerialPort::readAll();
    return tmp;
}



