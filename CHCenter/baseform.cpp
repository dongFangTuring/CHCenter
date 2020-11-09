#include "baseform.h"
#include "ui_baseform.h"

/**
 * @brief BaseForm::BaseForm -
 * set stylesheet, signals, ui when opening
 *
 *
 */


BaseForm::BaseForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BaseForm)
{

    ui->setupUi(this);


    /*hide the bottom StatusBar.*/
    this->setStatusBar(nullptr);

    /*set the stylesheet of baseform*/
    ui->LabelStatusMsg->setStyleSheet("background-color:#30302E; color: white; padding:15px 30px 15px 30px;");
    ui->SideBar->setStyleSheet("background-color:#30302E; color:white;");

    /*initial the HI221GW node choosing widget*/
    ui->ListGWNode->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->ListGWNode->setVisible(0);

    ch_serialport = new CHSerialport(nullptr);
    comform=new ComForm(this);
    comform->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    /*connect signals from class of CHSerialport and ComForm*/
    connect(comform, SIGNAL(sigPortChose(QString,int)), this, SLOT(getsigPortChose(QString,int)));
    connect(comform, SIGNAL(sigPortCancle()), this, SLOT(getsigPortCancle()));

    connect(ch_serialport, SIGNAL(errorOpenPort()), this, SLOT(geterrorOpenPort()));
    connect(ch_serialport, SIGNAL(sigOpenPort()), this, SLOT(getsigOpenPort()));
    connect(ch_serialport, SIGNAL(sigUpdateListGWNode()), this, SLOT(updateListGWNode()));


    /*Welcome message*/
    statusbar_msg.baudrate="";
    statusbar_msg.port="";
    statusbar_msg.current_status=tr("Unconnected");
    statusbar_msg.sw_version=tr("Software Version: ")+QString::number(0.1);
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());

    on_SideBarBTN1_clicked();

}

BaseForm::~BaseForm()
{
    delete ui;
}


/**
 * @brief BaseForm::on_SideBarBTN1/2/3/4_clicked
 * change page of stackedwidget
 */
void BaseForm::on_SideBarBTN1_clicked()
{
    SideBar_toggled(1);
    ui->stackedWidget->setCurrentIndex(0);
}
void BaseForm::on_SideBarBTN2_clicked()
{
    SideBar_toggled(2);
    ui->stackedWidget->setCurrentIndex(1);
}
void BaseForm::on_SideBarBTN3_clicked()
{
    SideBar_toggled(3);
    ui->stackedWidget->setCurrentIndex(2);
}
void BaseForm::on_SideBarBTN4_clicked()
{
    SideBar_toggled(4);
    ui->stackedWidget->setCurrentIndex(3);
}

/**
 * @brief BaseForm::SideBar_toggled
 * highlight the BTN by index
 */
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
 * 1. serialport is not opened:show comform
 * 2. serialport is opened:disconnect it
 * 3. while comform is opened, BTN is disabled
 */

void BaseForm::on_BTNConnect_clicked()
{
    if(!ch_serialport->CH_serial->isOpen()){
        comform->show();
    }
    else{
        ch_serialport->closeSerialport();

        statusbar_msg.baudrate="";
        statusbar_msg.port="";
        statusbar_msg.current_status=tr("Unconnected");
        statusbar_msg.sw_version=tr("Software Version: ")+QString::number(0.1);
        ui->LabelStatusMsg->setText(statusbar_msg.getMsg());
    }

    update_BTNConnect_state();
}

/**
 * @brief BaseForm::rec_port_chose -
 * linking to CH device, if port can't opened, thead will be stop and pop up an error message.
 * 1.sigOpenPort():getsigOpenPort() and start sigSendData()
 * 2.errorOpenPort():call geterrorOpenPort()
 */
void BaseForm::getsigPortChose(QString port_name, int baudrate)
{

    comform->hide();
    ch_serialport->linkCHdevices(port_name,baudrate);

    statusbar_msg.baudrate=QString::number(baudrate);
    statusbar_msg.port=port_name;
    statusbar_msg.current_status=tr("Connecting...");
    statusbar_msg.sw_version="";
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());
}
void BaseForm::geterrorOpenPort()
{
    showMessageBox(tr("Cannot build connection"),tr("Error"));

    comform->show();

    statusbar_msg.current_status=tr("Cannot build connection. Please check the selected port again");
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());
}
void BaseForm::getsigOpenPort()
{
    comform->hide();
    connect(ch_serialport, SIGNAL(sigSendData()),
            this, SLOT(getsigData()), Qt::QueuedConnection);

    update_BTNConnect_state();

    statusbar_msg.current_status=tr("Streaming...");
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());
}
void BaseForm::getsigPortCancle()
{
    comform->hide();
    update_BTNConnect_state();
}

/**
 * @brief BaseForm::getsigData
 * process 2 kinds of data: hi226/hi229/ch110/hi221 vs hi221gw
 * @param Is_gwsol:check if it is hi221gw.
 * @param current_gwnodeIndex: while streaming hi221gw, it will remember the chose node(by ID) until it's offine
 */
void BaseForm::getsigData()
{
    if(ch_serialport->Is_gwsol==0){
        receive_imusol_packet_t imuData=*ch_serialport->IMU_data;
        displayIMUnumber(imuData, ch_serialport->Content_bits);
    }
    else if(ch_serialport->Is_gwsol==1){
        receive_gwsol_packet_t imusData=*ch_serialport->IMUs_data;
        if(imusData.n>0){
            if(current_gwnodeIndex<0)
                displayIMUnumber(imusData.receive_imusol[0], ch_serialport->Content_bits);
            else{
                displayIMUnumber(imusData.receive_imusol[current_gwnodeIndex], ch_serialport->Content_bits);
            }
        }
    }
}

void BaseForm::update_BTNConnect_state()
{
    if(!ch_serialport->CH_serial->isOpen()){

        ui->BTNConnect->setText(tr("Connect"));

        if(comform->isVisible()){
            ui->BTNConnect->setEnabled(0);
        }
        else{
            ui->BTNConnect->setEnabled(1);
        }

    }
    else{
        ui->BTNConnect->setText(tr("Disconnect"));
        ui->BTNConnect->setEnabled(1);
    }
}

/**
 * @brief BaseForm::displayIMUnumber
 * @param imu_data:package from a single node
 * @param Content_bits:define what kinds of data can be showed
 */

void BaseForm::displayIMUnumber(receive_imusol_packet_t imu_data, unsigned int Content_bits)
{

    ui->LabelFrameRate->setText(QString::number(ch_serialport->Frame_rate) + " Hz");
    if(Content_bits & BIT_VALID_ID){
        if(!ui->LabelID->isVisible())
            ui->LabelID->setVisible(1);
        ui->LabelID->setText("ID = " + QString::number(imu_data.id));
    }
    else{
        if(ui->LabelID->isVisible())
            ui->LabelID->setVisible(0);
    }
    if(Content_bits & BIT_VALID_ACC){
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
    if(Content_bits & BIT_VALID_GYR){
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
    if(Content_bits & BIT_VALID_MAG){
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
    if(Content_bits & BIT_VALID_EUL){
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
    if(Content_bits & BIT_VALID_QUAT){
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

void BaseForm::on_actionExit_triggered()
{
    this->close();
}

/**
 * @brief BaseForm::on_actionTraditional_Chinese_triggered
 * write new language setting in CHCenter.ini near the CHCenter.exe
 */
void BaseForm::on_actionTraditional_Chinese_triggered()
{
    QFile file("CHCenter.ini");

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        QTextStream stream(&file);
        stream << "Language=tc" <<"\n";
        file.close();

    }
    else{
        QTextStream stream(&file);
        stream << "Language=tc" <<"\n";
        file.close();
    }
    showMessageBox(tr("Please restart software to activate new language."),tr("Information"));
}

void BaseForm::on_actionSimplified_Chinese_triggered()
{
    QFile file("CHCenter.ini");

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        QTextStream stream(&file);
        stream << "Language=cn" <<"\n";
        file.close();
    }
    else{
        QTextStream stream(&file);
        stream << "Language=cn" <<"\n";
        file.close();
    }
    showMessageBox(tr("Please restart software to activate new language."),tr("Information"));
}

void BaseForm::on_actionEnglish_triggered()
{
    QFile file("CHCenter.ini");

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        QTextStream stream(&file);
        stream << "Language=en" <<"\n";
        file.close();
    }
    else{
        QTextStream stream(&file);
        stream << "Language=en"<<"\n";
        file.close();
    }

    showMessageBox(tr("Please restart software to activate new language."),tr("Information"));

}

void BaseForm::showMessageBox(QString msg, QString title)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setWindowTitle(title);
    msgBox.exec();
}

/**
 * @brief BaseForm::updateListGWNode
 * if number of nodes changes send sigUpdateListGWNode():call updateListGWNode()
 */
void BaseForm::updateListGWNode()
{
    if(ch_serialport->Is_gwsol==1){
        ui->ListGWNode->clear();
        ui->ListGWNode->setVisible(1);

        receive_gwsol_packet_t imusData=*ch_serialport->IMUs_data;

        bool idexist=false;
        for(int i = 0; i < imusData.n; i++)
        {
            int t_id=imusData.receive_imusol[i].id;

            ui->ListGWNode->addItem(tr("Wireless Node ID : %1").arg(t_id));

            qDebug()<<t_id<< "^"<<current_gwnodeID;
            if(t_id==current_gwnodeID){
                ui->ListGWNode->setCurrentRow(i);
                current_gwnodeIndex=i;
                idexist=true;
            }

        }
        if(idexist==false)
            current_gwnodeIndex=-1;
    }
    else
        ui->ListGWNode->setVisible(0);
}

/**
 * @brief BaseForm::on_ListGWNode_itemClicked
 * @param current_gwnodeID:remember the chosen ID
 */
void BaseForm::on_ListGWNode_itemClicked(QListWidgetItem *item)
{
    QString id = ui->ListGWNode->currentItem()->text().split(" : ").last();

    current_gwnodeID=id.toInt();
    updateListGWNode();
}
