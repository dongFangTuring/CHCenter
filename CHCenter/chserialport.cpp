#include "chserialport.h"

CHSerialport::CHSerialport(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    m_serial = new QSerialPort();
    timer_framerate = new QTimer(this);
}

CHSerialport::~CHSerialport()
{
    closeSerialport();
}


int CHSerialport::initThreadReading()
{
    if(m_thread->isRunning())
        return -1;

    connect(m_thread, SIGNAL(started()), this, SLOT(on_thread_started()),Qt::QueuedConnection);
    connect(m_thread, SIGNAL(finished()), this, SLOT(on_thread_stopped()),Qt::QueuedConnection);
    connect(m_serial, SIGNAL(readyRead()), this, SLOT(handleData()), Qt::QueuedConnection);
    connect(timer_framerate, SIGNAL(timeout()), this, SLOT(countFrameRate()),Qt::QueuedConnection);

    this->moveToThread(m_thread);
    m_serial->moveToThread(m_thread);
    timer_framerate->moveToThread(m_thread);
    timer_framerate->setInterval(1000);
    m_thread->start();

    return 0;
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
    if(this->m_serial->isOpen()) {
        this->m_serial->disconnect();
        this->m_serial->close();
    }
    m_thread->quit();
    m_thread->wait();
    //m_thread->deleteLater();
    qDebug()<<"Port and thread are closed";
}

int CHSerialport::linkCHdevices(QString port_name, int baudrate)
{
    imu_data_decode_init();

    int ret=0;
    ret=this->openSerialport(port_name, baudrate);
    if(ret==-1){
        closeSerialport();
        return -1;
    }
    else{
        ret=this->initThreadReading();
        if(ret==-1){

            m_thread->quit();
            m_thread->wait();
            //m_thread->deleteLater();
            return -1;
        }
        else{
            return 0;
        }
    }

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
    timer_framerate->start();
}

void CHSerialport::on_thread_stopped()
{
    timer_framerate->stop();
    timer_framerate->disconnect();
}


void CHSerialport::handleData()
{

    long long NumberOfBytesToRead = m_serial->bytesAvailable();

    if(NumberOfBytesToRead > 0 && m_serial->isReadable())
    {
        QByteArray arr = m_serial->readAll();

        for (int i=0;i<NumberOfBytesToRead;i++) {
            uint8_t c=arr[i];
            packet_decode(c);
        }
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
    mutex_writing.unlock();

    emit sig_send_data();
}

void CHSerialport::write_data()
{
    qDebug() << "write_id is:" << QThread::currentThreadId();
    m_serial->write("data", 4);
}
