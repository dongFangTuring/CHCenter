#ifndef SERIAL_H
#define SERIAL_H


#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>


class serial:public QSerialPort
{
    Q_OBJECT
public:
    serial();
    ~serial();
    //保留父类的open函数并重载open函数
    using QSerialPort::open;
    bool open(QString PortName, int BaudRate);
    int64_t write(const QByteArray &data);
    QByteArray readAll();

    //刷新串口
    QList<QString> refreshSerialPort();

};


#endif // SERIAL_H
