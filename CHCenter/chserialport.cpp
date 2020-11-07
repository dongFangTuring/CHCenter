#include "chserialport.h"

CHSerialport::CHSerialport(QObject *parent) : QObject(parent)
{
    //this->setParent(nullptr);

    m_thread = new QThread();
    m_serial = new QSerialPort();
    timer_framerate = new QTimer();
    this->moveToThread(m_thread);
    m_serial->moveToThread(m_thread);
    timer_framerate->moveToThread(m_thread);
    connect(m_thread, SIGNAL(started()), this, SLOT(on_thread_started()),Qt::QueuedConnection);
    connect(m_thread, SIGNAL(finished()), this, SLOT(on_thread_stopped()),Qt::QueuedConnection);

    timer_framerate->setInterval(1000);


}

CHSerialport::~CHSerialport()
{
    closeSerialport();
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

    m_serial->setPortName(port_name);
    m_serial->setBaudRate(baudrate);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serial->open(QIODevice::ReadWrite)) {
        return -1;
    }
    return 0;
}

void CHSerialport::closeSerialport()
{
    if(m_serial->isOpen()) {
        m_serial->disconnect();
        m_serial->close();
    }
    m_thread->quit();
    m_thread->wait();
    //m_thread->deleteLater();
    qDebug()<<"Port and thread are closed";
}

void CHSerialport::linkCHdevices(QString port_name, int baudrate)
{
    imu_data_decode_init();

    m_port_name=port_name;
    m_baudrate=baudrate;
    initThreadReading();

}

void CHSerialport::countFrameRate()
{

    mutex_writing.lock();
    frame_rate=frame_count;
    mutex_writing.unlock();
    frame_count=0;

}

void CHSerialport::on_thread_started()
{

    connect(m_serial, SIGNAL(readyRead()), this, SLOT(handleData()), Qt::QueuedConnection);
    connect(timer_framerate, SIGNAL(timeout()), this, SLOT(countFrameRate()),Qt::QueuedConnection);

    timer_framerate->start();
    int ret=openSerialport(m_port_name, m_baudrate);
    if(ret==-1){
        closeSerialport();
        emit errorOpenPort();
    }
    else{
        emit sigOpenPort();
        qDebug() << "serial port thread is:" << QThread::currentThreadId();
    }
}

void CHSerialport::on_thread_stopped()
{
    timer_framerate->stop();
    timer_framerate->disconnect();
}


void CHSerialport::handleData()
{
    //qDebug() << "started thread is:" << QThread::currentThreadId();

    if(m_serial->bytesAvailable() > 0 && m_serial->isReadable())
    {
        long long NumberOfBytesToRead=m_serial->bytesAvailable();

        QByteArray arr = m_serial->readAll();

        for (int i=0;i<NumberOfBytesToRead;i++) {
            uint8_t c=arr[i];
            packet_decode(c);
        }

        mutex_writing.lock();
        if(receive_gwsol.tag != KItemGWSOL)
        {
            is_gwsol=0;
            IMU_data=&receive_imusol;
        }
        else
        {
            is_gwsol=1;
            for(int i = 0; i < receive_gwsol.n; i++)
            {
                IMUs_data=&receive_gwsol;
            }
        }
        if(m_bitmap!=bitmap)
            m_bitmap=bitmap;
        mutex_writing.unlock();

        emit sigSendData();
    }


}

void CHSerialport::write_data()
{
    qDebug() << "write_id is:" << QThread::currentThreadId();
    m_serial->write("data", 4);
}
