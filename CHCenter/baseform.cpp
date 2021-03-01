#include "baseform.h"
#include "ui_baseform.h"

/**
 * @brief BaseForm::BaseForm -
 * set stylesheet, signals, ui when opening
 *
 *
 */

QString sf_version="1.0.2";//major.minor.bugix

BaseForm::BaseForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BaseForm)
{

    ui->setupUi(this);





    this->setWindowTitle(tr("CH Center"));


    //hide the bottom StatusBar.
    this->setStatusBar(nullptr);

    //set the stylesheet of baseform
    ui->LabelStatusMsg->setStyleSheet("background-color:#30302E; color: white; padding:15px 30px 15px 30px;");
    ui->SideBar->setStyleSheet("background-color:#30302E; color:white;");
    //ui->stackedWidget->setStyleSheet("background-color:rgb(250,250,250);");


    //initial the HI221GW node choosing widget
    ui->DongleNodeList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->DongleNodeList->setVisible(false);


    ch_comform=new CHComForm(this);
    ch_comform->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    //get BTN signal from comport selecting form
    connect(ch_comform, SIGNAL(sigPortChose(QString,int)), this, SLOT(getsigPortChose(QString,int)));
    connect(ch_comform, SIGNAL(sigPortCancle()), this, SLOT(getsigPortCancle()));


    ch_serialport = new CHSerialport(nullptr);

    //get data from ch_serialport class
    connect(ch_serialport, SIGNAL(errorOpenPort()), this, SLOT(geterrorOpenPort()));
    connect(ch_serialport, SIGNAL(sigPortOpened()), this, SLOT(getsigPortOpened()));
    connect(ch_serialport, SIGNAL(sigPortClosed()), this, SLOT(getsigPortClosed()));
    connect(ch_serialport, SIGNAL(sigUpdateDongleNodeList(bool, receive_gwsol_packet_t)), this, SLOT(updateDongleNodeList(bool, receive_gwsol_packet_t)));
    connect(ch_serialport, SIGNAL(sigSendIMU(receive_imusol_packet_t)),
            this, SLOT(getIMUData(receive_imusol_packet_t)), Qt::QueuedConnection);
    connect(ch_serialport, SIGNAL(sigSendDongle(receive_gwsol_packet_t)),
            this, SLOT(getDongleData(receive_gwsol_packet_t)), Qt::QueuedConnection);
    connect(ch_serialport, SIGNAL(sigSendIMUmsg(QString)), this, SLOT(getIMUmsg(QString)));

    //page 1 widget initialize : Attitude indicator
    addADI();

    //a timer to update baseform
    baseform_timer=new QTimer(this);
    connect(baseform_timer, SIGNAL(timeout(void)), this, SLOT(updateBaseForm(void)));
    baseform_timer->setInterval(40);

    m_chartAcc=new ChartWindow(nullptr,"acc");
    m_chartGyr=new ChartWindow(nullptr,"gyr");
    m_chartMag=new ChartWindow(nullptr,"mag");
    m_chartEul=new ChartWindow(nullptr,"eul");
    m_chartQuat=new ChartWindow(nullptr,"quat");

    connect(this, SIGNAL(sigUpdateBaseFormChart(receive_imusol_packet_t)),
            this, SLOT(updateBaseFormChart(receive_imusol_packet_t)), Qt::DirectConnection);


    //page 2 widget initialize : 3D widget
    ch_threeDform=new ThreeDForm(this);
    ui->PageThreeDViewLayout->addWidget(ch_threeDform);

    //page 3
    ch_csvlogform=new CSVLogForm(this);
    ui->PageCSVLoggerLayout->addWidget(ch_csvlogform);
    connect(ch_serialport, SIGNAL(sigSendIMU(receive_imusol_packet_t)),
            ch_csvlogform, SLOT(getIMUData(receive_imusol_packet_t)));
    connect(ch_serialport, SIGNAL(sigSendDongle(receive_gwsol_packet_t)),
            ch_csvlogform, SLOT(getDongleData(receive_gwsol_packet_t)));
    connect(ch_serialport, SIGNAL(sigSendBitmap(uchar)),
            ch_csvlogform, SLOT(getBitmap(uchar)));
    //    connect(ch_serialport, SIGNAL(sigPortClosed()),
    //            ch_csvlogform, SLOT(stopLogging()));

    //page 4
    ch_settingform=new CHSettingForm(this);
    ui->PageSettingWidget->addWidget(ch_settingform);
    connect(ch_settingform,SIGNAL(sigSendATcmd(QString)), this, SLOT(getsigSendATcmd(QString)));

    //about form
    m_aboutform = new AboutForm(this);
    m_aboutform->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    m_aboutform->setFixedSize(500,500);


    //Welcome message
    statusbar_msg.baudrate="";
    statusbar_msg.port="";
    statusbar_msg.current_status=tr("Unconnected");
    statusbar_msg.sw_version=tr("Software Version : %1").arg(sf_version);
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());


    //set page 1 as the default page
    on_SideBarBTN1_clicked();
    update_BTNConnect_state();
    ui->stackedWidget->setEnabled(false);
}

void BaseForm::closeEvent (QCloseEvent *event)
{
    //    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "CH Center",
    //                                                                tr("Are you sure to exit?\n"),
    //                                                                QMessageBox::Cancel | QMessageBox::Yes);
    //    if (resBtn != QMessageBox::Yes) {
    //        event->ignore();
    //    } else {
    ch_serialport->closePort();
    m_ADI->deleteLater();
    m_Compass->deleteLater();

    //serial port
    ch_comform->deleteLater();
    ch_serialport->deleteLater();


    ch_settingform->deleteLater();
    ch_threeDform->deleteLater();
    ch_csvlogform->deleteLater();

    //charts
    m_chartAcc->close();
    m_chartGyr->close();
    m_chartMag->close();
    m_chartEul->close();
    m_chartQuat->close();

    m_aboutform->close();
    event->accept();
    //    }
}
BaseForm::~BaseForm()
{
    delete ui;
}

///In SideBar///
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

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
    if(ui->SideBarBTN4->isEnabled()==false){
        ch_settingform->settingConfig_leave();
    }


    ui->SideBarBTN1->setEnabled(true);
    ui->SideBarBTN2->setEnabled(true);
    ui->SideBarBTN3->setEnabled(true);
    ui->SideBarBTN4->setEnabled(true);


    if(index==1){
        baseform_timer->start();
    }
    else{
        baseform_timer->stop();
    }

    if(index==2){
        ch_threeDform->startThreeDPlot();
        connect(this, SIGNAL(sigSendIMUtoThreeD(receive_imusol_packet_t)),
                ch_threeDform, SLOT(getIMUData(receive_imusol_packet_t)));
    }
    else{
        ch_threeDform->stopThreeDPlot();
        disconnect(this, SIGNAL(sigSendIMUtoThreeD(receive_imusol_packet_t)),
                   ch_threeDform, SLOT(getIMUData(receive_imusol_packet_t)));
    }


    switch (index) {
    case 1: {
        ch_settingform->StreamATcmd();
        ch_serialport->Is_msgMode=0;
        ui->SideBarBTN1->setEnabled(false);
        break;
    }
    case 2: {
        ch_settingform->StreamATcmd();
        ch_serialport->Is_msgMode=0;
        ui->SideBarBTN2->setEnabled(false);
        break;
    }
    case 3: {
        ch_settingform->StreamATcmd();
        ch_serialport->Is_msgMode=0;
        ui->SideBarBTN3->setEnabled(false);
        break;
    }
    case 4: {
        getsigSendATcmd("AT+EOUT=0");
        ch_settingform->settingConfig_init();
        ch_serialport->Is_msgMode=1;
        ui->SideBarBTN4->setEnabled(false);
        break;
    }
    default:
        break;
    }
}

/**
 * @brief BaseForm::on_BTNConnect_clicked -
 * 1. serialport is not opened:show ch_comform
 * 2. serialport is opened:disconnect it
 * 3. while ch_comform is opened, BTN is disabled
 */
void BaseForm::on_BTNConnect_clicked()
{
    if(!ch_serialport->CH_serial->isOpen()){
        ch_comform->show();
        ch_comform->on_BTNPortRefresh_clicked();
    }

    update_BTNConnect_state();

    receive_gwsol_packet_t empty;
    updateDongleNodeList(false,empty);
}

void BaseForm::on_BTNDisconnect_clicked()
{
    if(ch_serialport->CH_serial->isOpen()){
        ch_serialport->closePort();
        receive_gwsol_packet_t empty;
        updateDongleNodeList(false,empty);
    }
}

void BaseForm::update_BTNConnect_state()
{
    if(!ch_serialport->CH_serial->isOpen()){
        ui->BTNConnect->setFixedWidth(200);
        if(ch_comform->isVisible()){
            ui->BTNConnect->setEnabled(false);
        }
        else{
            ui->BTNConnect->setEnabled(true);
            ui->BTNDisconnect->hide();
        }

    }
    else{
        ui->BTNConnect->setFixedWidth(150);
        ui->BTNConnect->setEnabled(false);
        ui->BTNDisconnect->show();
    }
}

/**
 * @brief BaseForm::updateDongleNodeList
 * if number of nodes changes send sigUpdateDongleNodeList():call updateDongleNodeList()
 */
void BaseForm::updateDongleNodeList(bool m_is_dongle, receive_gwsol_packet_t dongle_data)
{
    if(m_is_dongle==1){
        ui->DongleNodeList->clear();
        ui->DongleNodeList->setVisible(true);


        if(dongle_data.n==0){
            ui->DongleNodeList->addItem(tr("No node is online."));

        }
        else{
            bool idexist=false;
            for(int i = 0; i < dongle_data.n; i++)
            {
                int t_id=dongle_data.receive_imusol[i].id;

                ui->DongleNodeList->addItem(tr("Wireless Node ID : %1").arg(t_id));

                if(t_id==cur_dongle_nodeID){
                    ui->DongleNodeList->setCurrentRow(i);
                    cur_dongle_nodeIndex=i;
                    idexist=true;
                }

            }
            if(idexist==false)
                cur_dongle_nodeIndex=0;
        }

    }
    else
        ui->DongleNodeList->setVisible(false);
}

/**
 * @brief BaseForm::on_DongleNodeList_itemClicked
 * @param cur_dongle_nodeID:remember the chosen ID
 */
void BaseForm::on_DongleNodeList_itemClicked(QListWidgetItem *item)
{
    QString id = ui->DongleNodeList->currentItem()->text().split(" : ").last();

    cur_dongle_nodeID=id.toInt();
    cur_dongle_nodeIndex=ui->DongleNodeList->currentRow();

}

///signal from ch_comform ui///
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief BaseForm::rec_port_chose -
 * linking to CH device, if port can't opened, thead will be stop and pop up an error message.
 * 1.sigPortOpened():getsigPortOpened() and start sigSendData()
 * 2.errorOpenPort():call geterrorOpenPort()
 */
void BaseForm::getsigPortChose(QString port_name, int baudrate)
{

    ch_comform->hide();
    ch_serialport->linkCHdevices(port_name,baudrate);

    statusbar_msg.baudrate=QString::number(baudrate);
    statusbar_msg.port=port_name;
    statusbar_msg.current_status=tr("Connecting...");
    statusbar_msg.sw_version="";
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());
}

void BaseForm::getsigPortCancle()
{
    ch_comform->close();
    update_BTNConnect_state();
}


///signal from chserial class///
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

void BaseForm::geterrorOpenPort()
{
    showMessageBox(tr("Cannot build connection"),tr("Error"));

    ch_comform->show();

    statusbar_msg.current_status=tr("Cannot build connection. Please check the selected port again");
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());
    update_BTNConnect_state();
    ui->stackedWidget->setEnabled(false);
}
void BaseForm::getsigPortOpened()
{
    ch_comform->hide();

    //"connect" BTN
    update_BTNConnect_state();

    //statusbar
    statusbar_msg.current_status=tr("Streaming...");
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());

    //send at+eout
    ch_settingform->StreamATcmd();
    ui->stackedWidget->setEnabled(true);

}

void BaseForm::getsigPortClosed()
{

    //close serial thread by main thread
    ch_serialport->quitmThread();

    //statusbar
    statusbar_msg.baudrate="";
    statusbar_msg.port="";
    statusbar_msg.current_status=tr("Unconnected");
    statusbar_msg.sw_version=tr("Software Version : %1").arg(sf_version);
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());

    update_BTNConnect_state();
    ui->stackedWidget->setEnabled(false);
}

/**
 * @brief BaseForm::getIMUData, BaseForm::getDongleData
 * process 2 kinds of data: hi226/hi229/ch110/hi221 vs hi221gw
 * @param cur_dongle_nodeIndex: while streaming hi221gw, it will remember the chose node(by ID) until it's offine
 */
void BaseForm::getIMUData(receive_imusol_packet_t imu_data)
{

    mutex_writing.lock();

    m_imu_data=imu_data;
    m_contentbits = ch_serialport->Content_bits;

    mutex_writing.unlock();

    emit sigUpdateBaseFormChart(imu_data);
    emit sigSendIMUtoThreeD(imu_data);

}
void BaseForm::getDongleData(receive_gwsol_packet_t dongle_data)
{    

    //Connected number of nodes > 0
    if(dongle_data.n>0){

        receive_imusol_packet_t imu_data;

        imu_data=dongle_data.receive_imusol[cur_dongle_nodeIndex];


        mutex_writing.lock();

        m_imu_data=imu_data;
        m_contentbits = ch_serialport->Content_bits;
        m_protocol_tag = dongle_data.tag;

        mutex_writing.unlock();

        emit sigUpdateBaseFormChart(imu_data);
        emit sigSendIMUtoThreeD(imu_data);
    }
    //No node is connected.
    else{
        m_contentbits=0;
        m_protocol_tag=dongle_data.tag;
    }
}

/**
 * @brief BaseForm::updateBaseForm()
 * I use timer to update.
 */
void BaseForm::updateBaseForm()
{

    updateIMUTable(m_imu_data, m_contentbits, m_protocol_tag);
    m_ADI->setData(m_imu_data.eul[0],m_imu_data.eul[1]);
    m_Compass->setYaw(m_imu_data.eul[2]);
    m_protocol_tag=0;
}


/**
 * @brief BaseForm::getIMUmsg
 * when go into setting mode, node will transmit ACHii instead of HEX data.
 * @param str:message from node, BAUD is a special case because user need to re-connect manually.
 */
void BaseForm::getIMUmsg(QString str)
{
    ch_settingform->setTerminalBoxText(str);
    if(str.indexOf("BAUD")>=0){
        statusbar_msg.current_status=tr("Please restart the device to take effect, and connect with new Baudrate.");
        ui->LabelStatusMsg->setText(statusbar_msg.getMsg());
    }
}

void BaseForm::getsigSendATcmd(QString ATcmd)
{
    ATcmd+="\r\n";
    ch_serialport->writeData(ATcmd);
}
///stackwidget page1 content:data, chart and attitude indicator///
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

void BaseForm::addADI()
{
    m_ADI      = new QADI();
    m_Compass  = new QCompass();

    ui->DataTopBar->addWidget(m_ADI);
    ui->DataTopBar->addWidget(m_Compass);
}

/**
 * @brief BaseForm::updateIMUTable
 * @param imu_data:package from a single node
 * @param Content_bits:define what kinds of data can be showed
 */

void BaseForm::updateIMUTable(receive_imusol_packet_t imu_data, uchar content_bits, uchar protocol_tag)
{

    QString setptl="";
    ui->LabelFrameRate->setText(QString::number(ch_serialport->Frame_rate) + " Hz");

    if(content_bits & BIT_VALID_ID){
        if(!ui->LabelID->isVisible())
            ui->LabelID->setVisible(true);
        ui->LabelID->setText("ID = " + QString::number(imu_data.id));
        setptl+="90,";
    }
    else{
        if(ui->LabelID->isVisible())
            ui->LabelID->setVisible(false);

    }
    if(content_bits & BIT_VALID_ACC){
        if(!ui->LabelGPAcc->isVisible()){
            ui->LabelGPAcc->setVisible(true);
        }
        ui->LabelAccX->setText(QString::number(imu_data.acc[0],'f',3));
        ui->LabelAccY->setText(QString::number(imu_data.acc[1],'f',3));
        ui->LabelAccZ->setText(QString::number(imu_data.acc[2],'f',3));
        setptl+="A0,";
    }
    else{
        if(ui->LabelGPAcc->isVisible()){
            ui->LabelGPAcc->setVisible(false);
        }
    }
    if(content_bits & BIT_VALID_GYR){
        if(!ui->LabelGPGyro->isVisible())
            ui->LabelGPGyro->setVisible(true);
        ui->LabelGyroX->setText(QString::number(imu_data.gyr[0],'f',3));
        ui->LabelGyroY->setText(QString::number(imu_data.gyr[1],'f',3));
        ui->LabelGyroZ->setText(QString::number(imu_data.gyr[2],'f',3));
        setptl+="B0,";
    }
    else{
        if(ui->LabelGPGyro->isVisible())
            ui->LabelGPGyro->setVisible(false);
    }
    if(content_bits & BIT_VALID_MAG){
        if(!ui->LabelGPMag->isVisible())
            ui->LabelGPMag->setVisible(true);
        ui->LabelMagX->setText(QString::number(imu_data.mag[0],'f',0));
        ui->LabelMagY->setText(QString::number(imu_data.mag[1],'f',0));
        ui->LabelMagZ->setText(QString::number(imu_data.mag[2],'f',0));
        setptl+="C0,";
    }
    else{
        if(ui->LabelGPMag->isVisible())
            ui->LabelGPMag->setVisible(false);
    }
    if(content_bits & BIT_VALID_EUL){
        if(!ui->LabelGPEuler->isVisible())
            ui->LabelGPEuler->setVisible(true);
        ui->LabelEulerX->setText(QString::number(imu_data.eul[0],'f',2));
        ui->LabelEulerY->setText(QString::number(imu_data.eul[1],'f',2));
        ui->LabelEulerZ->setText(QString::number(imu_data.eul[2],'f',2));
        setptl+="D0,";
    }
    else{
        if(ui->LabelGPEuler->isVisible())
            ui->LabelGPEuler->setVisible(false);
    }
    if(content_bits & BIT_VALID_QUAT){
        if(!ui->LabelGPQuat->isVisible())
            ui->LabelGPQuat->setVisible(true);
        ui->LabelQuatW->setText(QString::number(imu_data.quat[0],'f',3));
        ui->LabelQuatX->setText(QString::number(imu_data.quat[1],'f',3));
        ui->LabelQuatY->setText(QString::number(imu_data.quat[2],'f',3));
        ui->LabelQuatZ->setText(QString::number(imu_data.quat[3],'f',3));
        setptl+="D1,";
    }
    else{
        if(ui->LabelGPQuat->isVisible())
            ui->LabelGPQuat->setVisible(false);
    }

    if(content_bits == BIT_VALID_ALL){
        setptl="91";
    }
    if(protocol_tag == KItemDongleRaw)
        setptl="63";

    else if (protocol_tag == KItemDongle)
        setptl="62";



    //delete that last comma
    int ret=setptl.lastIndexOf(',');
    if(ret!=-1)
        setptl.remove(ret,1);

    ui->LabelDataProtocol->setText(tr("Data Protocol = %1").arg(setptl));

}

void BaseForm::updateBaseFormChart(receive_imusol_packet_t imu_data)
{

    if(m_contentbits & BIT_VALID_ACC){
        m_chartAcc->updateLineData(imu_data.acc);
    }
    else{
        m_chartAcc->setVisible(false);
    }
    if(m_contentbits & BIT_VALID_GYR){
        m_chartGyr->updateLineData(imu_data.gyr);
    }
    else{
        m_chartGyr->setVisible(false);
    }
    if(m_contentbits & BIT_VALID_MAG){
        m_chartMag->updateLineData(imu_data.mag);
    }
    else{
        m_chartMag->setVisible(false);
    }
    if(m_contentbits & BIT_VALID_EUL){
        m_chartEul->updateLineData(imu_data.eul);
    }
    else{
        m_chartEul->setVisible(false);
    }
    if(m_contentbits & BIT_VALID_QUAT){
        m_chartQuat->updateLineData(imu_data.quat);
    }
    else{
        m_chartQuat->setVisible(false);
    }

}



///MenuBar signals of actions///
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

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

void BaseForm::on_actionFAQ_triggered()
{
    QDesktopServices::openUrl(QUrl("https://hipnuc.com/datahub.html"));
}

void BaseForm::on_actionAbout_triggered()
{
    m_aboutform->show();
}

///StatusBar information///
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

void BaseForm::showMessageBox(QString msg, QString title)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setWindowTitle(title);
    msgBox.exec();
}




///Chart BTN///
////////////////////////////////////////////////////////////////////////////////////////

void BaseForm::on_BTNChartAcc_clicked()
{
    if(!m_chartAcc->isVisible()){
        m_chartAcc->setVisible(true);
        m_chartAcc->init();
    }
    else
        m_chartAcc->setVisible(false);
}

void BaseForm::on_BTNChartGyr_clicked()
{
    if(!m_chartGyr->isVisible()){
        m_chartGyr->setVisible(true);
        m_chartGyr->init();
    }
    else
        m_chartGyr->setVisible(false);
}

void BaseForm::on_BTNChartMag_clicked()
{
    if(!m_chartMag->isVisible()){
        m_chartMag->setVisible(true);
        m_chartMag->init();
    }
    else
        m_chartMag->setVisible(false);
}

void BaseForm::on_BTNChartEul_clicked()
{
    if(!m_chartEul->isVisible()){
        m_chartEul->setVisible(true);
        m_chartEul->init();
    }
    else
        m_chartEul->setVisible(false);
}

void BaseForm::on_BTNChartQuat_clicked()
{
    if(!m_chartQuat->isVisible()){
        m_chartQuat->setVisible(true);
        m_chartQuat->init();
    }
    else
        m_chartQuat->setVisible(false);
}

