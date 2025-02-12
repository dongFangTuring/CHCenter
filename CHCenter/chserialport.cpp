#include "chserialport.h"

CHSerialport::CHSerialport(QObject *parent) : QSerialPort(parent)
{
    //this->setParent(nullptr); this is unnecessary
    m_thread = new QThread();

    CH_serial = new QSerialPort();
    timer_framerate = new QTimer();

    m_kboot = new kboot_protocol();
    m_mdbus = new mdbus();

    //move those to second thread
    this->moveToThread(m_thread);
    CH_serial->moveToThread(m_thread);
    timer_framerate->moveToThread(m_thread);
    m_kboot->moveToThread(m_thread);
    m_mdbus->moveToThread(m_thread);

    /* connect kboot paser to serial */
    connect(this, SIGNAL(sig_send_kboot(QByteArray&)), m_kboot, SLOT(slt_serial_read(QByteArray&)));
    connect(m_kboot, SIGNAL(sig_serial_send(QByteArray&)), this, SLOT(slt_serial_send(QByteArray&)));
    /* connect kptl recv to form_display */
    connect(m_kboot, SIGNAL(sig_frame_recv(QByteArray&)), this, SLOT(protocol_0x5A(QByteArray&)));


    /* connect modbus decoder to serial */
    connect(this, SIGNAL(sig_send_bus(QByteArray&)), m_mdbus, SLOT(slt_serial_read(QByteArray&)));
    connect(m_mdbus, SIGNAL(sig_serial_send(QByteArray&)), this, SLOT(slt_serial_send(QByteArray&)));


    //cross threads signals
    connect(m_thread, SIGNAL(started()), this, SLOT(on_thread_started()),Qt::QueuedConnection);
    connect(m_thread, SIGNAL(finished()), this, SLOT(on_thread_stopped()),Qt::QueuedConnection);

    //be able to write data from baseform to serial port in second thread
    //connect(this, SIGNAL(sigWriteData(QString)), this, SLOT(getsigWriteData(QString)));

    //be able to close port and thread from baseform
    connect(this, SIGNAL(sigCloseThreadAndPort()), this, SLOT(closeThreadAndPort()));

    timer_framerate->setInterval(1000);

}

CHSerialport::~CHSerialport()
{
    sigCloseThreadAndPort();
}


int CHSerialport::openSerialport(QString port_name, int baudrate)
{

    CH_serial->setPortName(port_name);
    CH_serial->setBaudRate(baudrate);
    CH_serial->setDataBits(QSerialPort::Data8);
    CH_serial->setParity(QSerialPort::NoParity);
    CH_serial->setStopBits(QSerialPort::OneStop);
    CH_serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!CH_serial->open(QIODevice::ReadWrite)) {
        return -1;
    }
    return 0;
}

void CHSerialport::closePort()
{
    emit sigCloseThreadAndPort();
}

void CHSerialport::initThreadReading()
{
    if(m_thread->isRunning()) {
        m_thread->quit();
        m_thread->wait();
    }
    m_thread->start(); //go to on thread started

}

void CHSerialport::closeThreadAndPort()
{
    while(1) {
        if(CH_serial->isOpen()) {
            CH_serial->disconnect();
            CH_serial->close();
        } else {
            emit sigPortClosed();
            break;
        }
    }
}

void CHSerialport::quitmThread()
{
    m_thread->quit();
    m_thread->wait();
    qDebug()<<"Port and thread are closed";
}

void CHSerialport::linkCHdevices(QString port_name, int baudrate)
{
    m_port_name = port_name;
    m_baudrate = baudrate;
    initThreadReading();
}

void CHSerialport::countFrameRate()
{

    mutex_writing.lock();
    Frame_rate = m_frame_counter;
    m_frame_counter = 0;
    mutex_writing.unlock();
    if(Frame_rate == 0)
    {
        checkPortStatus();
    }

}

void CHSerialport::checkPortStatus()
{
    QSerialPortInfo *info = new QSerialPortInfo;
    QList<QSerialPortInfo> list = info->availablePorts();
    bool port_still_available = false;

    for (QSerialPortInfo &port : list) {
        if (port.portName() == CH_serial->portName())
            port_still_available = true;
    }
    if (!port_still_available) {
        qDebug() << "Selected COM-port," << CH_serial->portName() << ", no longer available!";
        closePort();
        emit errorOpenPort();
    }
}

void CHSerialport::on_thread_started()
{

    connect(CH_serial, SIGNAL(readyRead()), this, SLOT(handleData()), Qt::QueuedConnection);
    connect(timer_framerate, SIGNAL(timeout()), this, SLOT(countFrameRate()),Qt::QueuedConnection);

    m_kboot->kboot_clear();


    timer_framerate->start();
    int ret = openSerialport(m_port_name, m_baudrate);
    if(ret == -1) {
        closePort();
        emit errorOpenPort();
    } else {
        emit sigPortOpened();
        //qDebug() << "serial port thread is:" << QThread::currentThreadId();
    }
}

void CHSerialport::on_thread_stopped()
{
    timer_framerate->stop();
    timer_framerate->disconnect();

    parser.dev_info.node_cnt=0;
    m_node_cnt=0;
    m_isDongle=0;
}




void CHSerialport::handleData()
{

    if(CH_serial->bytesAvailable() > 0 && CH_serial->isReadable()) {
        QByteArray raw_data = CH_serial->readAll();

        emit sig_send_kboot(raw_data);
        emit sig_send_bus(raw_data);

        protocol_ASC2(raw_data);
    }


}
void CHSerialport::protocol_ASC2(QByteArray asc2_data)
{
    static QString m_IMUmsg;
    int rst=asc2_data.indexOf(0x5A);

    //has found 0x5A
    if(rst!=-1) {
//        m_IMUmsg=QString(asc2_data.toHex()).toUpper();
//        QString a = m_IMUmsg.replace(QRegularExpression("(.{2})"), "\\1 ");
//        emit sigSendIMUmsg(tr("Rx : %1").arg(a));
//        m_IMUmsg="";
    }

    //no found of 0x5A, meaning that the device sent ACS2
    else {
        m_IMUmsg+=asc2_data;
        if(m_IMUmsg.indexOf("OK")!=-1) {
            emit sigSendIMUmsg(m_IMUmsg);
            m_IMUmsg="";
        } else if(m_IMUmsg.indexOf("ERR")!=-1) {
            emit sigSendIMUmsg(m_IMUmsg);
            m_IMUmsg="";
        } else {
            if(m_IMUmsg.size()>300) {
                emit sigSendIMUmsg("DECODE_ERR");
                m_IMUmsg="";
            }

        }
    }
}
void CHSerialport::protocol_0x5A(QByteArray &binary_data)
{
    static uchar m_bitmap=0;

    mutex_writing.lock();

    parser.parse(binary_data);

    //counting frames per seconds
    m_frame_counter++;

    IMU_packets.clear();
    IMU_packets.resize(0);


    if(parser.bitmap & BIT_RF_DONGLE) {

        for(int i=0; i<parser.dev_info.node_cnt; i++) {
            IMU_packets.append(parser.dev[i]);
        }

        //if switch from single IMU module
        if(m_isDongle==0) {
            m_isDongle=1;
            emit sigUpdateDongleNodeList(true, IMU_packets);
        }

        //if the new numbers of nodes isn't equal to the list
        if(!(m_node_cnt==parser.dev_info.node_cnt)) {
            m_node_cnt=parser.dev_info.node_cnt;
            emit sigUpdateDongleNodeList(true, IMU_packets);

        }
        emit sigSendIMU(IMU_packets);

    } else { /* not RF doongle flag, only single IMU */
        parser.dev_info.node_cnt = 1;
        IMU_packets.append(parser.dev[0]);

        if(m_isDongle==1) {
            m_isDongle=0;
            emit sigUpdateDongleNodeList(false, IMU_packets);
        }
        emit sigSendIMU(IMU_packets);

    }

    if(m_bitmap!=parser.bitmap) {
        m_bitmap=parser.bitmap;
        emit sigSendBitmap(m_bitmap);
    }

    mutex_writing.unlock();
}

void CHSerialport::slt_writeData(QString ATcmd)
{
    QByteArray ba = ATcmd.toLocal8Bit();
    const char *c_str2 = ba.data();
    CH_serial->write(c_str2,100);

}
void CHSerialport:: slt_serial_send(QByteArray &ba)
{
    CH_serial->write(ba);
    QString a=ba.toHex().toUpper();
    QString split = a.replace(QRegularExpression("(.{2})"), "\\1 ");

    //emit sigSendIMUmsg(tr("Tx : %1").arg(split));
    //this->mserial->clear(QSerialPort::Input);

    CH_serial->waitForBytesWritten();
    //this->mserial->waitForReadyRead(20);
}

void CHSerialport::sltRWMdbus(char rw, uint32_t *param, int16_t address)
{
    /* read product info */
    if(rw=='r') {
        /* FIXME
         * read a dummy first
         * */
        m_mdbus->read_data(0, 0, &param[0], 1);
        if(address<0){
            for(int i=0; i<112; i+=16) { /* suggest not to read all param, use what, read what */
                bool ret = m_mdbus->read_data(0, i, &param[i], 16);
                if(ret) {
                    emit sigMdbusParamLoaded();
                } else {
                    qDebug()<<"MDBUS: READ ERROR:"<<i;
                }
            }
        }else if(address>=0) {
            bool ret = m_mdbus->read_reg(0, address,*param);
            if(ret) {

            } else {
                qDebug()<<"MDBUS: READ ERROR:"<<address;
            }
        }


    } else if (rw=='w') {
        if(address<0) { //write all
            for(unsigned short int i=0; i<112; i=i+16) {
                m_mdbus->write_data(0, i, &param[i], 16);
            }
        }

        else if(address>=0) {
            bool isOK = m_mdbus->write_reg(0, address,*param);
            //qDebug()<<*param;
            if(!isOK) {
                qDebug()<<"MDBUS: WRITE ERROR";
            }
        }

    }

}
