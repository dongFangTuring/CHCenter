#include <QObject>
#include <QSerialPort>
#include <QString>
#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QObject>
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

  int openSerialport(QString, int);
  QSerialPort *m_serial = nullptr;
  receive_imusol_packet_t *IMU_data;
  receive_gwsol_packet_t *IMUs_data;
  unsigned int frame_rate=0;
  bool is_gwsol=0;

public slots:
  void handleData();
  void write_data();
  int initThreadReading();
  void closeSerialport();
  int linkCHdevices(QString, int);

signals:
  void sig_send_data();

private:
  QThread *m_thread;
  QTimer *timer_framerate;
  QMutex mutex_writing;

private slots:
  void countFrameRate();
  void on_thread_started();
  void on_thread_stopped();


};
