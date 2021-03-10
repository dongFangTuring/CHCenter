#include "chserialport.h"

CHSerialport::CHSerialport(QObject *parent) : QObject(parent)
{
    //this->setParent(nullptr); this is unnecessary
    m_thread = new QThread();

    CH_serial = new QSerialPort();
    timer_framerate = new QTimer();

    kboot = new kboot_protocol();
    bus_reader = new mdbus();


    //move those to second thread
    this->moveToThread(m_thread);
    CH_serial->moveToThread(m_thread);
    timer_framerate->moveToThread(m_thread);
    kboot->moveToThread(m_thread);

    /* connect kboot paser to serial */
    connect(this, SIGNAL(sig_send_kbootbus(QByteArray&)), this->kboot, SLOT(slt_serial_read(QByteArray&)));
    connect(kboot, SIGNAL(sig_serial_send(QByteArray&)), this, SLOT(slt_serial_send(QByteArray&)));


    /* connect kboot download progress */
    //connect(this->kboot, &kboot_protocol::sig_download_progress, this, &MainWindow::slt_update_progress_bar);

    /* connect modbus decoder to serial */
    connect(this, SIGNAL(sig_send_kbootbus(QByteArray&)), bus_reader, SLOT(slt_serial_read(QByteArray&)));
    connect(bus_reader, SIGNAL(sig_serial_send(QByteArray&)), this, SLOT(slt_serial_send(QByteArray&)));


    /* connect kptl recv to form_display */
    connect(kboot, SIGNAL(sig_frame_recv(QByteArray&)), this, SLOT(protocol_0x5A(QByteArray&)));


    //cross threads signals
    connect(m_thread, SIGNAL(started()), this, SLOT(on_thread_started()),Qt::QueuedConnection);
    connect(m_thread, SIGNAL(finished()), this, SLOT(on_thread_stopped()),Qt::QueuedConnection);

    //be able to write data from baseform to serial port in second thread
    connect(this, SIGNAL(sigWriteData(QString)), this, SLOT(getsigWriteData(QString)));

    //be able to close port and thread from baseform
    connect(this, SIGNAL(sigCloseThreadAndPort()), this, SLOT(closeThreadAndPort()));


    timer_framerate->setInterval(1000);


}

CHSerialport::~CHSerialport()
{
    sigCloseThreadAndPort();
}


void CHSerialport::initThreadReading()
{
    if(m_thread->isRunning()){
        m_thread->quit();
        m_thread->wait();
    }
    m_thread->start();

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

void CHSerialport::closePort(){
    emit sigCloseThreadAndPort();
}

void CHSerialport::closeThreadAndPort()
{
    while(1){
        if(CH_serial->isOpen()) {
            CH_serial->disconnect();
            CH_serial->close();
        }
        else {
            emit sigPortClosed();
            break;
        }
    }
}
void CHSerialport::quitmThread(){
    m_thread->quit();
    m_thread->wait();
    qDebug()<<"Port and thread are closed";
}



void CHSerialport::linkCHdevices(QString port_name, int baudrate)
{

    m_port_name=port_name;
    m_baudrate=baudrate;
    initThreadReading();
}

void CHSerialport::countFrameRate()
{

    mutex_writing.lock();
    Frame_rate=m_frame_counter;
    m_frame_counter=0;
    mutex_writing.unlock();
    if(Frame_rate==0){
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

    timer_framerate->start();
    int ret=openSerialport(m_port_name, m_baudrate);
    if(ret==-1){
        closePort();
        emit errorOpenPort();
    }
    else{
        emit sigPortOpened();
        //qDebug() << "serial port thread is:" << QThread::currentThreadId();
    }


}
void CHSerialport::on_thread_stopped()
{
    timer_framerate->stop();
    timer_framerate->disconnect();

    IMU_data.dev_info.node_cnt=0;
    m_node_cnt=0;

    m_is_gwsol=0;
}

void CHSerialport::getsigWriteData(QString str)
{
    QByteArray ba = str.toLocal8Bit();
    const char *c_str2 = ba.data();
    CH_serial->write(c_str2,100);

}


void CHSerialport::handleData()
{

    if(CH_serial->bytesAvailable() > 0 && CH_serial->isReadable())
    {
        QByteArray raw_data = CH_serial->readAll();


        protocol_ASC2(raw_data);

        //kboot protocol_0x5A;
        emit sig_send_kbootbus(raw_data);
        //mdbus reader
        //        uint32_t buf[64];
        //        int aaa = bus_reader->read_data(1, 0, buf, 32);
        //        if(aaa){
        //            for(int i=0; i<32; i++){
        //                qDebug()<<buf[i];
        //            }
        //        }

    }


}

void CHSerialport::protocol_0x5A(QByteArray &binary_data)
{
    mutex_writing.lock();

    IMU_data.parse(binary_data);

    //counting frames per seconds
    m_frame_counter++;

    IMU_packets.clear();
    IMU_packets.resize(0);


    if(IMU_data.bitmap & BIT_RF_DONGLE)
    {
        for(int i=0;i<IMU_data.dev_info.node_cnt;i++){
            IMU_packets.append(IMU_data.dev[i]);
        }

        //if switch from single IMU module
        if(m_is_gwsol==0){
            m_is_gwsol=1;
            emit sigUpdateDongleNodeList(true, IMU_packets);
        }

        //if the new numbers of nodes isn't equal to the list
        if(!(m_node_cnt==IMU_data.dev_info.node_cnt)){
            m_node_cnt=IMU_data.dev_info.node_cnt;
            emit sigUpdateDongleNodeList(true, IMU_packets);

        }
        emit sigSendDongle(IMU_packets);

    }
    else /* not RF doongle flag, only single IMU */
    {
        IMU_data.dev_info.node_cnt = 1;
        IMU_packets.append(IMU_data.dev[0]);

        if(m_is_gwsol==1){
            m_is_gwsol=0;
            emit sigUpdateDongleNodeList(false, IMU_packets);
        }

        emit sigSendIMU(IMU_data.dev[0]);

    }

    if(Content_bits!=IMU_data.bitmap){
        Content_bits=IMU_data.bitmap;
        sigSendBitmap(Content_bits);
    }

    mutex_writing.unlock();

    //    for (int i=0;i<binary_data.length();i++) {
    //        uint8_t c=binary_data[i];
    //        packet_decode(c);
    //    }

    //    //if there is new frame, send signal with the new frame to baseform and other classes.
    //    if(m_frame_received!=frame_count){


    //        if(receive_gwsol.tag == KItemDongle || receive_gwsol.tag == KItemDongleRaw)
    //        {
    //            //if switch from single IMU module
    //            if(m_is_gwsol==0){
    //                m_is_gwsol=1;
    //                emit sigUpdateDongleNodeList(true,receive_gwsol);
    //            }

    //            //if the new numbers of nodes isn't equal to the list
    //            if(!(m_node_cnt==receive_gwsol.n)){
    //                m_node_cnt=receive_gwsol.n;
    //                emit sigUpdateDongleNodeList(true,receive_gwsol);
    //            }

    //            emit sigSendDongle(receive_gwsol);

    //        }
    //        else{
    //            if(m_is_gwsol==1){
    //                m_is_gwsol=0;
    //                emit sigUpdateDongleNodeList(false,receive_gwsol);
    //            }

    //            emit sigSendIMU(receive_imusol);

    //        }


    //        if(Content_bits!=bitmap){
    //            Content_bits=bitmap;
    //            sigSendBitmap(Content_bits);
    //        }
    //        m_frame_received=frame_count;

    //    }
}

void CHSerialport::protocol_ASC2(QByteArray asc2_data)
{
    int rst=asc2_data.indexOf(0x5A);

    //has found 0x5A
    if(rst!=-1){
        m_IMUmsg=QString(asc2_data.toHex()).toUpper();
        emit sigSendIMUmsg(m_IMUmsg);
        m_IMUmsg="";
    }

    //no found of 0x5A, meaning that the device sent ACS2
    else{
        m_IMUmsg+=asc2_data;
        if(m_IMUmsg.indexOf("OK")!=-1)
        {
            emit sigSendIMUmsg(m_IMUmsg);
            m_IMUmsg="";
        }
        else if(m_IMUmsg.indexOf("ERR")!=-1){
            emit sigSendIMUmsg(m_IMUmsg);
            m_IMUmsg="";
        }
        else{
            if(m_IMUmsg.size()>300){
                emit sigSendIMUmsg("Data decoded error.");
                m_IMUmsg="";
            }

        }


    }
}

void CHSerialport::writeData(QString ATcmd)
{
    emit sigWriteData(ATcmd);
}
void CHSerialport:: slt_serial_send(QByteArray &ba)
{
    CH_serial->write(ba);

    //    this->mserial->clear(QSerialPort::Input);

    CH_serial->waitForBytesWritten();
    //this->mserial->waitForReadyRead(20);
}
