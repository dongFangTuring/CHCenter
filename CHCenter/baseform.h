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
    void rec_port_chose(QString port_name,int baudrate);
    void rec_port_cancle();
    void rec_data();
    void update_BTNConnect_state();
    void display_IMUnumber(receive_imusol_packet_t IMU_data);

private:
    Ui::BaseForm *ui;
    ComForm *comform;
    CHSerialport *ch_serialport;
};
#endif // BASEFORM_H
