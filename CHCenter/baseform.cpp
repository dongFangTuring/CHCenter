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
    this->setStatusBar(nullptr);




    ch_serialport = new CHSerialport(this);
    comform=new ComForm(this);
    comform->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    connect(comform, SIGNAL(sig_port_chose(QString,int)), this, SLOT(rec_port_chose(QString,int)));
    connect(comform, SIGNAL(sig_port_cancle()), this, SLOT(rec_port_cancle()));

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
    }

    update_BTNConnect_state();
}

/**
 * @brief BaseForm::rec_port_chose -
 * @param ret: result of linking to CH device, if ret=-1, thead will be clean and pop up an error message.
 * if ret=0, means connection is build successfully.
 */

void BaseForm::rec_port_chose(QString port_name, int baudrate)
{
    qDebug()<<port_name;

    int ret=ch_serialport->linkCHdevices(port_name,baudrate);

    if(ret==0){
        connect(ch_serialport, SIGNAL(sig_send_data()),
                this, SLOT(rec_data()), Qt::QueuedConnection);
        comform->hide();
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Cannot connect to " + port_name);
        msgBox.setWindowTitle("Error");
        msgBox.exec();
    }

    update_BTNConnect_state();
}

void BaseForm::rec_port_cancle()
{
    comform->hide();
    update_BTNConnect_state();
}

void BaseForm::rec_data()
{
    if(ch_serialport->is_gwsol==0){
        display_IMUnumber(*ch_serialport->IMU_data);
    }
    ui->LabelFrameRate->setText("Frame Rate = " + QString::number(ch_serialport->frame_rate) + " Hz");
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

void BaseForm::display_IMUnumber(receive_imusol_packet_t IMU_data)
{
    ui->LabelAccX->setText(QString::number(IMU_data.acc[0],'f',3));
    ui->LabelAccY->setText(QString::number(IMU_data.acc[1],'f',3));
    ui->LabelAccZ->setText(QString::number(IMU_data.acc[2],'f',3));

    ui->LabelGyroX->setText(QString::number(IMU_data.gyr[0],'f',3));
    ui->LabelGyroY->setText(QString::number(IMU_data.gyr[1],'f',3));
    ui->LabelGyroZ->setText(QString::number(IMU_data.gyr[2],'f',3));

    ui->LabelMagX->setText(QString::number(IMU_data.mag[0],'f',0));
    ui->LabelMagY->setText(QString::number(IMU_data.mag[1],'f',0));
    ui->LabelMagZ->setText(QString::number(IMU_data.mag[2],'f',0));

    ui->LabelEulerX->setText(QString::number(IMU_data.eul[0],'f',1));
    ui->LabelEulerY->setText(QString::number(IMU_data.eul[1],'f',1));
    ui->LabelEulerZ->setText(QString::number(IMU_data.eul[2],'f',1));

    ui->LabelQuatW->setText(QString::number(IMU_data.quat[0],'f',3));
    ui->LabelQuatX->setText(QString::number(IMU_data.quat[1],'f',3));
    ui->LabelQuatY->setText(QString::number(IMU_data.quat[2],'f',3));
    ui->LabelQuatZ->setText(QString::number(IMU_data.quat[3],'f',3));
}
