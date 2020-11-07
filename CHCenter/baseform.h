#ifndef BASEFORM_H
#define BASEFORM_H

#include <QMainWindow>
#include "packet/imu_data_decode.h"
#include "packet/packet.h"
#include "comform.h"
#include "chserialport.h"


QT_BEGIN_NAMESPACE
namespace Ui { class BaseForm; }
QT_END_NAMESPACE

class BaseForm : public QMainWindow
{
    Q_OBJECT

public:
    BaseForm(QWidget *parent = nullptr);
    ~BaseForm();

private slots:
    void on_SideBarBTN1_clicked();
    void on_SideBarBTN2_clicked();
    void on_SideBarBTN3_clicked();
    void on_SideBarBTN4_clicked();
    void SideBar_toggled(int index);

    void on_BTNConnect_clicked();
    void getsigPortChose(QString port_name,int baudrate);
    void getsigPortCancle();
    void getsigData();

    void geterrorOpenPort();
    void getsigOpenPort();

    void update_BTNConnect_state();
    void displayIMUnumber(receive_imusol_packet_t imu_data, unsigned int m_bitmap);



private:
    Ui::BaseForm *ui;
    ComForm *comform;
    CHSerialport *ch_serialport;

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
