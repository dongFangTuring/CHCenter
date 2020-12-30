#ifndef BASEFORM_H
#define BASEFORM_H

#include <QMainWindow>
#include <QLabel>
#include <QFile>
#include "packet/imu_data_decode.h"
#include "packet/packet.h"
#include "chcomform.h"
#include "chserialport.h"
#include "attitudeindicator.h"
#include "chsettingform.h"
#include "threedform.h"
#include "csvlogform.h"
#include "chartwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class BaseForm; }
QT_END_NAMESPACE

class BaseForm : public QMainWindow
{
    Q_OBJECT

public:
    BaseForm(QWidget *parent = nullptr);
    ~BaseForm();

signals:
    void sigUpdateBaseFormChart(receive_imusol_packet_t, uchar);
    void sigSendIMUtoThreeD(receive_imusol_packet_t);


private slots:

    ///In SideBar///
    void on_SideBarBTN1_clicked();
    void on_SideBarBTN2_clicked();
    void on_SideBarBTN3_clicked();
    void on_SideBarBTN4_clicked();
    void SideBar_toggled(int index);
    void on_BTNConnect_clicked();
    void on_BTNDisconnect_clicked();
    void update_BTNConnect_state();

    //this will be show if device is HI221GW
    void updateDongleNodeList(bool);
    void on_DongleNodeList_itemClicked(QListWidgetItem *item);

    ///signal from CHComForm ui///
    void getsigPortChose(QString port_name,int baudrate);
    void getsigPortCancle();

    ///signal from CHSerialPort class///
    void geterrorOpenPort();
    void getsigPortOpened();
    void getsigPortClosed();
    void getIMUData(receive_imusol_packet_t);
    void getDongleData(receive_gwsol_packet_t);
    void getIMUmsg(QString);

    ///stackwidget page1 content:data, chart and attitude indicator///
    void updateBaseForm();
    void addADI();
    void updateIMUTable(receive_imusol_packet_t, uchar, uchar);
    void updateBaseFormChart(receive_imusol_packet_t, uchar);

    ///stackwidget page2 content:///



    ///stackwidget page3 content:///


    ///stackwidget page4 content:///
    void getsigSendATcmd(QString ATcmd);

    ///MenuBar signals of actions///
    void on_actionExit_triggered();
    void on_actionTraditional_Chinese_triggered();
    void on_actionSimplified_Chinese_triggered();
    void on_actionEnglish_triggered();

    ///StatusBar///
    void showMessageBox(QString msg, QString title);

    void on_BTNChartAcc_clicked();

    void on_BTNChartGyr_clicked();

    void on_BTNChartMag_clicked();

    void on_BTNChartEul_clicked();

    void on_BTNChartQuat_clicked();

private:
    Ui::BaseForm *ui;

    QTimer *baseform_timer;

    //IMU node and dongle
    receive_imusol_packet_t m_imu_data;
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
