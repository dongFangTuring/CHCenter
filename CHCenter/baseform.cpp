#include "baseform.h"
#include "ui_baseform.h"

BaseForm::BaseForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BaseForm)
{
    ui->setupUi(this);

    /*set the background-color of the baseform.*/

    this->setStyleSheet("background-color:#EEEEEE;");
    ui->SideBar->setStyleSheet("background-color:#30302E;");

    /*hide the bottom StatusBar.*/
    //this->setStatusBar(nullptr);

    this->statusBar()->setStyleSheet("background-color:#30302E; color: white;");


    ch_serialport = new CHSerialport(nullptr);
    comform=new ComForm(this);
    comform->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    connect(comform, SIGNAL(sigPortChose(QString,int)), this, SLOT(getsigPortChose(QString,int)));
    connect(comform, SIGNAL(sigPortCancle()), this, SLOT(getsigPortCancle()));

    connect(ch_serialport, SIGNAL(errorOpenPort()), this, SLOT(geterrorOpenPort()));
    connect(ch_serialport, SIGNAL(sigOpenPort()), this, SLOT(getsigOpenPort()));

    statusbar_msg.baudrate="";
    statusbar_msg.port="";
    statusbar_msg.current_status="Unconnected";
    statusbar_msg.sw_version="Software Version: "+QString::number(0.1);
    ui->statusbar->showMessage(statusbar_msg.getMsg());

}

BaseForm::~BaseForm()
{
    delete ui;
}


void BaseForm::on_SideBarBTN1_clicked()
{
    SideBar_toggled(1);
}
void BaseForm::on_SideBarBTN2_clicked()
{
    SideBar_toggled(2);
}
void BaseForm::on_SideBarBTN3_clicked()
{
    SideBar_toggled(3);
}
void BaseForm::on_SideBarBTN4_clicked()
{
    SideBar_toggled(4);
}

void BaseForm::SideBar_toggled(int index)
{
    ui->SideBarBTN1->setEnabled(1);
    ui->SideBarBTN2->setEnabled(1);
    ui->SideBarBTN3->setEnabled(1);
    ui->SideBarBTN4->setEnabled(1);

    switch (index) {
    case 1: ui->SideBarBTN1->setEnabled(0);
        break;
    case 2: ui->SideBarBTN2->setEnabled(0);
        break;
    case 3: ui->SideBarBTN3->setEnabled(0);
        break;
    case 4: ui->SideBarBTN4->setEnabled(0);
        break;
    default:
        break;

    }
}

/**
 * @brief BaseForm::on_BTNConnect_clicked -
 * after create a new ComForm, we can connect signals and slot between two classes.
 */

void BaseForm::on_BTNConnect_clicked()
{
    if(!ch_serialport->m_serial->isOpen()){

        comform->show();
    }
    else{
        ch_serialport->closeSerialport();

        statusbar_msg.baudrate="";
        statusbar_msg.port="";
        statusbar_msg.current_status="Unconnected";
        statusbar_msg.sw_version="Software Version: "+QString::number(0.1);
        ui->statusbar->showMessage(statusbar_msg.getMsg());
    }

    update_BTNConnect_state();
}

/**
 * @brief BaseForm::rec_port_chose -
 * @param ret: result of linking to CH device, if ret=-1, thead will be clean and pop up an error message.
 * if ret=0, means connection is build successfully.
 */

void BaseForm::getsigPortChose(QString port_name, int baudrate)
{

    comform->hide();
    ch_serialport->linkCHdevices(port_name,baudrate);

    statusbar_msg.baudrate=QString::number(baudrate);
    statusbar_msg.port=port_name;
    statusbar_msg.current_status="Connecting...";
    statusbar_msg.sw_version="";
    ui->statusbar->showMessage(statusbar_msg.getMsg());

}
void BaseForm::geterrorOpenPort()
{
    QMessageBox msgBox;
    msgBox.setText("Cannot build connection");
    msgBox.setWindowTitle("Error");
    msgBox.exec();
    comform->show();

    statusbar_msg.current_status="Cannot build connection. Please check the selected port again";
    ui->statusbar->showMessage(statusbar_msg.getMsg());
}

void BaseForm::getsigOpenPort()
{
    comform->hide();
    connect(ch_serialport, SIGNAL(sigSendData()),
            this, SLOT(getsigData()), Qt::QueuedConnection);

    update_BTNConnect_state();

    statusbar_msg.current_status="Streaming...";
    ui->statusbar->showMessage(statusbar_msg.getMsg());
}

void BaseForm::getsigPortCancle()
{
    comform->hide();
    update_BTNConnect_state();
}

void BaseForm::getsigData()
{

    receive_imusol_packet_t imuData=*ch_serialport->IMU_data;


    if(ch_serialport->is_gwsol==0){
        displayIMUnumber(imuData, ch_serialport->m_bitmap);
    }


}


void BaseForm::update_BTNConnect_state()
{
    if(!ch_serialport->m_serial->isOpen()){

        ui->BTNConnect->setText("Connect");

        if(comform->isVisible()){
            ui->BTNConnect->setEnabled(0);
        }
        else{
            ui->BTNConnect->setEnabled(1);
        }

    }
    else{
        ui->BTNConnect->setText("Disconnect");
        ui->BTNConnect->setEnabled(1);
    }
}

void BaseForm::displayIMUnumber(receive_imusol_packet_t imu_data, unsigned int m_bitmap)
{

    ui->LabelFrameRate->setText("Frame Rate = " + QString::number(ch_serialport->frame_rate) + " Hz");
    if(m_bitmap & BIT_VALID_ID){
        if(!ui->LabelID->isVisible())
            ui->LabelID->setVisible(1);
        ui->LabelID->setText("ID = " + QString::number(imu_data.id));
    }
    else{
        if(ui->LabelID->isVisible())
            ui->LabelID->setVisible(0);
    }
    if(m_bitmap & BIT_VALID_ACC){
        if(!ui->LabelGPAcc->isVisible()){
            ui->LabelGPAcc->setVisible(1);
        }
        ui->LabelAccX->setText(QString::number(imu_data.acc[0],'f',3));
        ui->LabelAccY->setText(QString::number(imu_data.acc[1],'f',3));
        ui->LabelAccZ->setText(QString::number(imu_data.acc[2],'f',3));
    }
    else{
        if(ui->LabelGPAcc->isVisible()){
            ui->LabelGPAcc->setVisible(0);
        }

    }
    if(m_bitmap & BIT_VALID_GYR){
        if(!ui->LabelGPGyro->isVisible())
            ui->LabelGPGyro->setVisible(1);
        ui->LabelGyroX->setText(QString::number(imu_data.gyr[0],'f',3));
        ui->LabelGyroY->setText(QString::number(imu_data.gyr[1],'f',3));
        ui->LabelGyroZ->setText(QString::number(imu_data.gyr[2],'f',3));
    }
    else{
        if(ui->LabelGPGyro->isVisible())
            ui->LabelGPGyro->setVisible(0);
    }
    if(m_bitmap & BIT_VALID_MAG){
        if(!ui->LabelGPMag->isVisible())
            ui->LabelGPMag->setVisible(1);
        ui->LabelMagX->setText(QString::number(imu_data.mag[0],'f',0));
        ui->LabelMagY->setText(QString::number(imu_data.mag[1],'f',0));
        ui->LabelMagZ->setText(QString::number(imu_data.mag[2],'f',0));
    }
    else{
        if(ui->LabelGPMag->isVisible())
            ui->LabelGPMag->setVisible(0);
    }
    if(m_bitmap & BIT_VALID_EUL){
        if(!ui->LabelGPEuler->isVisible())
            ui->LabelGPEuler->setVisible(1);
        ui->LabelEulerX->setText(QString::number(imu_data.eul[0],'f',1));
        ui->LabelEulerY->setText(QString::number(imu_data.eul[1],'f',1));
        ui->LabelEulerZ->setText(QString::number(imu_data.eul[2],'f',1));
    }
    else{
        if(ui->LabelGPEuler->isVisible())
            ui->LabelGPEuler->setVisible(0);
    }
    if(m_bitmap & BIT_VALID_QUAT){
        if(!ui->LabelGPQuat->isVisible())
            ui->LabelGPQuat->setVisible(1);
        ui->LabelQuatW->setText(QString::number(imu_data.quat[0],'f',3));
        ui->LabelQuatX->setText(QString::number(imu_data.quat[1],'f',3));
        ui->LabelQuatY->setText(QString::number(imu_data.quat[2],'f',3));
        ui->LabelQuatZ->setText(QString::number(imu_data.quat[3],'f',3));
    }
    else{
        if(ui->LabelGPQuat->isVisible())
            ui->LabelGPQuat->setVisible(0);
    }
}
