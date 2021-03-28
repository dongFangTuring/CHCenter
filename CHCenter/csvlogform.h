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
#include <QCloseEvent>

#include "utilities/imu_parser.h"

namespace Ui {
class CSVLogForm;
}

class CSVLogForm : public QWidget
{
    Q_OBJECT

public:
    explicit CSVLogForm(QWidget *parent = nullptr);
    ~CSVLogForm() override;


private slots:

    //BTN
    void on_BTNPath_clicked();
    void on_BTNStart_clicked();
    void on_BTNStop_clicked();
    void on_BTNClear_clicked();

    //Get data from baseform
    void getIMUPackets(QVector<id0x91_t>);
    void getBitmap(uchar);

    void on_SBLogPeriod_valueChanged(int arg1);

    void startLogging();
    void stopLogging();

    //check if any frame is logged in 1 second
    void checkIsStarted();

    void logging_countdown();


protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::CSVLogForm *ui;
    QString current_dir;

    id0x91_t m_imudata;
    QVector<id0x91_t> m_gwimudata;

    QTimer *timer_log_period;
    QTimer *timer_countdown;

    bool log_started;
    bool log_paused;

    unsigned long long frame_counter;

    QString filename_logging;
    QFile ch_logfile;

    int countdown_sec;

    QVector<int> gwnode_idlist;

    void writeDongle2csv(QVector<id0x91_t>);
    void write2csv(id0x91_t);

    QString imudata2csvrow(id0x91_t);

    uchar m_bitmap;


};

#endif // CSVLOGFORM_H
