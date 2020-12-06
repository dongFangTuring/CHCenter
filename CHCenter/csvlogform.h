#ifndef CSVLOGFORM_H
#define CSVLOGFORM_H

#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QThread>

#include "packet/packet.h"
#include "packet/imu_data_decode.h"

namespace Ui {
class CSVLogForm;
}

class CSVLogForm : public QWidget
{
    Q_OBJECT

public:
    explicit CSVLogForm(QWidget *parent = nullptr);
    ~CSVLogForm();


private slots:
    void on_BTNPath_clicked();

    void on_BTNStart_clicked();

    void getIMUData(receive_imusol_packet_t);

    void getGWIMUData(receive_gwsol_packet_t);

    void on_BTNStop_clicked();

    void on_SBLogPeriod_valueChanged(int arg1);

    void getBitmap(unsigned int);

    void startLogging();
    void stopLogging();
    void logging_countdown();


    void on_BTNClear_clicked();

private:
    Ui::CSVLogForm *ui;
    QString current_dir;

    receive_imusol_packet_t m_imudata;
    receive_gwsol_packet_t m_gwimudata;

    QTimer *timer_log_period;
    QTimer *timer_countdown;

    bool log_started;
    bool log_paused;

    unsigned long long frame_counter;

    QString filename_logging;
    QFile ch_logfile;

    int countdown_sec;

    std::vector<int> gwnode_idlist;

    QString imudata2csvrow(receive_imusol_packet_t);

    unsigned int m_bitmap;


};

#endif // CSVLOGFORM_H
