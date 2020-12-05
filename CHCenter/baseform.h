#ifndef BASEFORM_H
#define BASEFORM_H

#include <QMainWindow>
#include <QLabel>
#include <QFile>

#include "packet/imu_data_decode.h"
#include "packet/packet.h"
#include "comform.h"
#include "chserialport.h"
#include "attitudeindicator.h"
#include "chsettingform.h"
#include "threedform.h"
#include "csvlogform.h"

#define PAGE_DATA           0
#define PAGE_THREED         1
#define PAGE_CSV_LOGGER     2
#define PAGE_CH_SETTING     3



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
    void sigPage1Display(receive_imusol_packet_t, int);
    void sigSendIMUtoThreeD(receive_imusol_packet_t);


private slots:

    ///In SideBar///
    void on_SideBarBTN1_clicked();
    void on_SideBarBTN2_clicked();
    void on_SideBarBTN3_clicked();
    void on_SideBarBTN4_clicked();
    void SideBar_toggled(int index);
    void on_BTNConnect_clicked();
    void update_BTNConnect_state();

    //this will be show if device is HI221GW
    void updateListGWNode(bool);
    void on_ListGWNode_itemClicked(QListWidgetItem *item);

    ///signal from comform ui///
    void getsigPortChose(QString port_name,int baudrate);
    void getsigPortCancle();

    ///signal from chserial class///
    void geterrorOpenPort();
    void getsigOpenPort();
    void getsigPortClosed();
    void getIMUData(receive_imusol_packet_t);
    void getGWIMUData(receive_gwsol_packet_t);
    void getIMUmsg(QString);

    ///stackwidget page1 content:data, chart and attitude indicator///
    void getsigBaseFormDisplay(receive_imusol_packet_t, int);
    void addADI();
    void displayIMUnumber(receive_imusol_packet_t, unsigned int, int);

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




private:
    Ui::BaseForm *ui;
    ComForm *comform;
    CHSerialport *ch_serialport;

    int current_gwnodeID=0;
    int current_gwnodeIndex;

    QADI *m_ADI;
    QCompass *m_Compass;

    CHSettingForm *ch_settingform;
    ThreeDForm *ch_threeDform;
    CSVLogForm *ch_csvlogform;



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
