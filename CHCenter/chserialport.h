#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QMessageBox>
#include <QTimer>
#include <QMutex>


#include "kptl/kboot_protocol.h"
#include "utilities/imu_parser.h"
#include "mdbus/mdbus.h"

class CHSerialport : public QObject
{
    Q_OBJECT


public:
    explicit CHSerialport(QObject *parent = nullptr);
    ~CHSerialport();

    QSerialPort *CH_serial = nullptr;
    int openSerialport(QString, int);
    uint Frame_rate=0;

    uchar Content_bits;

    imu_parser parser;

public slots:

    void closePort();
    void linkCHdevices(QString, int);
    void quitmThread();

signals:
    //emit data to baseform
    void sigSendDongle(QVector<id0x91_t>);
    void sigSendIMU(id0x91_t);
    void sigSendIMUmsg(QString);
    void sigSendBitmap(uchar);

    //port status handle
    void errorOpenPort();
    void sigPortOpened();
    void sigPortClosed();
    void sigCloseThreadAndPort();
    void sigUpdateDongleNodeList(bool, QVector<id0x91_t>);

    //send raw binary to kboot and modbus
    void sig_send_kboot(QByteArray&);
    void sig_send_bus(QByteArray&);

    //send back the read modbus config chsetting
    void sigMdbusParamLoaded();

private slots:
    //count Hz
    void countFrameRate();

    //if device is connected but 0Hz, check if it is still available
    void checkPortStatus();

    //control of the second thread
    void on_thread_started();
    void on_thread_stopped();
    void initThreadReading();
    void closeThreadAndPort();


    //handle all data from serial
    void handleData();
    void protocol_0x5A(QByteArray&);
    void protocol_ASC2(QByteArray);

    //write to serial, a cross thread command
    void slt_writeData(QString);
    void slt_serial_send(QByteArray &ba);

    void sltRWMdbus(char rw, uint32_t *param, int16_t address=-1);


private:

    QTimer *timer_framerate;
    QMutex mutex_writing;
    QString m_port_name;
    QThread *m_thread;
    int m_baudrate;

    int m_node_cnt;
    bool m_is_gwsol=0;

    QString m_IMUmsg="";

    uint m_frame_counter=0;

    kboot_protocol *m_kboot;
    mdbus *m_mdbus;

    QVector<id0x91_t> IMU_packets;





};
