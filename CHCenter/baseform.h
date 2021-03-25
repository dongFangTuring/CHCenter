#ifndef BASEFORM_H
#define BASEFORM_H

#include <QMainWindow>
#include <QLabel>
#include <QFile>
#include <QCloseEvent>
#include <QGraphicsScene>


#include "chcomform.h"
#include "chserialport.h"
#include "attitudeindicator.h"
#include "chsettingform.h"
#include "threedform.h"
#include "csvlogform.h"
#include "chartwindow.h"
#include "aboutform.h"



QT_BEGIN_NAMESPACE
namespace Ui { class BaseForm; }
QT_END_NAMESPACE

class BaseForm : public QMainWindow
{
    Q_OBJECT

public:
    BaseForm(QWidget *parent = nullptr);
    ~BaseForm() override;
protected:
    virtual void closeEvent (QCloseEvent *event) override;


signals:
    void sigUpdateBaseFormChart(id0x91_t);
    void sigSendIMUtoThreeD(id0x91_t);


private slots:

    ///In Bottom Bar///

    void on_BTNConnect_clicked();
    void on_BTNDisconnect_clicked();
    void update_BTNConnect_state();

    //this will be show if device is HI221GW
    void updateDongleNodeList(bool, QVector<id0x91_t>);
    void on_DongleNodeList_itemClicked(QListWidgetItem *item);

    ///signal from CHComForm ui///
    void getsigPortChose(QString port_name,int baudrate);
    void getsigPortCancle();

    ///signal from CHSerialPort class///
    void geterrorOpenPort();
    void getsigPortOpened();
    void getsigPortClosed();
    void getIMUPackets(QVector<id0x91_t> packets);
    void getBitmap(uchar);

    void getIMUmsg(QString);

    ///stackwidget page1 content:data, chart and attitude indicator///
    void updateBaseForm();
    void updateIMUTable(id0x91_t imu_data, uchar content_bits);
    void updateBaseFormChart(id0x91_t);

    ///stackwidget page2 content:///


    ///stackwidget page3 content:///


    ///stackwidget page4 content:///


    ///MenuBar signals of actions///
    void on_actionExit_triggered();
    void on_actionTraditional_Chinese_triggered();
    void on_actionSimplified_Chinese_triggered();
    void on_actionEnglish_triggered();
    void on_actionFAQ_triggered();
    void on_actionAbout_triggered();

    ///StatusBar///
    void showMessageBox(QString msg, QString title);
    void on_BTNChartAcc_clicked();
    void on_BTNChartGyr_clicked();
    void on_BTNChartMag_clicked();
    void on_BTNChartEul_clicked();
    void on_BTNChartQuat_clicked();


    void on_action3D_view_triggered();

    void on_actionCSV_Logger_triggered();

    void on_actionDevice_Settiing_triggered();

private:
    Ui::BaseForm *ui;

    QTimer *baseform_timer;

    //IMU node and dongle
    id0x91_t m_imu_data;
    uchar m_protocol_tag;
    uchar m_contentbits=0;

    QMutex mutex_writing;

    //ADI
    QADI *m_ADI;
    QCompass *m_Compass;

    //serial port
    CHComForm *ch_comform;
    CHSerialport *ch_serialport;
    int cur_dongle_nodeID=0;
    int cur_dongle_nodeIndex;

    CHSettingForm *ch_settingform;
    ThreeDForm *ch_threeDform;
    CSVLogForm *ch_csvlogform;

    //charts
    ChartWindow *m_chartAcc;
    ChartWindow *m_chartGyr;
    ChartWindow *m_chartMag;
    ChartWindow *m_chartEul;
    ChartWindow *m_chartQuat;

    AboutForm *m_aboutform;

    //information in status bar
    struct StatusbarMsg{
        QString sw_version="";
        QString port="";
        QString baudrate="";
        QString current_status="";

        QString getMsg(void){

            QString full_msg="";

            if(!sw_version.isEmpty())
                full_msg+=sw_version+". ";
            if(!port.isEmpty())
                full_msg+=port+". ";
            if(!baudrate.isEmpty())
                full_msg+="Baud="+baudrate+". ";
            if(!current_status.isEmpty())
                full_msg+=current_status;
            return full_msg;
        }
    };

    StatusbarMsg statusbar_msg;


};
#endif // BASEFORM_H
