#include <QObject>
#include <QSerialPort>
#include <QString>
#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QMessageBox>
#include <QTimer>
#include <QMutex>

#include "packet/packet.h"
#include "packet/imu_data_decode.h"

class CHSerialport : public QObject
{
    Q_OBJECT


public:
    explicit CHSerialport(QObject *parent = nullptr);
    ~CHSerialport();


    QSerialPort *CH_serial = nullptr;
    int openSerialport(QString, int);


    receive_imusol_packet_t *IMU_data=&receive_imusol;
    receive_gwsol_packet_t *IMUs_data=&receive_gwsol;
    uint Frame_rate=0;
    uchar Content_bits;


    QByteArray CH_rawmsg="";
    bool Is_msgMode=0;

public slots:
    void writeData(QString);
    void closePort();
    void linkCHdevices(QString, int);
    void quitmThread();

signals:
    //emit data to baseform
    void sigSendDongle(receive_gwsol_packet_t);
    void sigSendIMU(receive_imusol_packet_t);
    void sigSendIMUmsg(QString);
    void sigSendBitmap(uchar);

    //port status handle
    void errorOpenPort();
    void sigOpenPort();
    void sigPortClosed();
    void sigCloseThreadAndPort();
    void sigUpdateDongleList(bool);

    //write to serial, a cross thread command
    void sigWriteData(QString);   


private:
    QTimer *timer_framerate;
    QMutex mutex_writing;
    QString m_port_name;
    QThread *m_thread;
    int m_baudrate;
    int m_number_of_node;
    bool m_is_gwsol=0;
    QString m_IMUmsg="";

    uint m_frame_received=0;

private slots:
    void countFrameRate();
    void on_thread_started();
    void on_thread_stopped();
    void initThreadReading();
    void closeThreadAndPort();

    //handle all data from serial
    void handleData();

    //write to serial, a cross thread command
    void getsigWriteData(QString);


};
