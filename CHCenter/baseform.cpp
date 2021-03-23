#include "baseform.h"
#include "ui_baseform.h"

/**
 * @brief BaseForm::BaseForm -
 * set stylesheet, signals, ui when opening
 *
 *
 */

QString sf_version = "1.1.0"; //major.minor.bugix

BaseForm::BaseForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BaseForm)
{

    ui->setupUi(this);

    this->setWindowTitle(tr("CH Center"));


    //hide the bottom StatusBar.
    this->setStatusBar(nullptr);

    //set the stylesheet of baseform

    ui->LabelStatusMsg->setStyleSheet("background-color:white; color: black; padding:10px 25px;");
    ui->SideBar->setStyleSheet("background-color:#30302E; color:white;");
    ui->PageDataScroll->setStyleSheet("background-color:rgb(250, 250, 250);");


    //initial the HI221GW node choosing widget
    ui->DongleNodeList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->DongleNodeList->setVisible(false);


    ch_comform = new CHComForm(this);
    ch_comform->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    //get BTN signal from comport selecting form
    connect(ch_comform, SIGNAL(sigPortChose(QString, int)), this, SLOT(getsigPortChose(QString, int)));
    connect(ch_comform, SIGNAL(sigPortCancle()), this, SLOT(getsigPortCancle()));


    ch_serialport = new CHSerialport(nullptr);

    //get data from ch_serialport class
    connect(ch_serialport, SIGNAL(errorOpenPort()), this, SLOT(geterrorOpenPort()));
    connect(ch_serialport, SIGNAL(sigPortOpened()), this, SLOT(getsigPortOpened()));
    connect(ch_serialport, SIGNAL(sigPortClosed()), this, SLOT(getsigPortClosed()));
    connect(ch_serialport, SIGNAL(sigUpdateDongleNodeList(bool, QVector<id0x91_t>)),
            this, SLOT(updateDongleNodeList(bool, QVector<id0x91_t>)));
    connect(ch_serialport, SIGNAL(sigSendIMU(QVector<id0x91_t>)),
            this, SLOT(getIMUPackets(QVector<id0x91_t>)), Qt::QueuedConnection);
    connect(ch_serialport, SIGNAL(sigSendBitmap(uchar)),
            this, SLOT(getBitmap(uchar)));


    connect(ch_serialport, SIGNAL(sigSendIMUmsg(QString)), this, SLOT(getIMUmsg(QString)));

    //page 1 widget initialize : Attitude indicator

    //a timer to update baseform
    baseform_timer = new QTimer(this);
    connect(baseform_timer, SIGNAL(timeout(void)), this, SLOT(updateBaseForm(void)));
    baseform_timer->setInterval(50);
    baseform_timer->start();

    m_chartAcc = new ChartWindow(nullptr, "acc");
    m_chartGyr = new ChartWindow(nullptr, "gyr");
    m_chartMag = new ChartWindow(nullptr, "mag");
    m_chartEul = new ChartWindow(nullptr, "eul");
    m_chartQuat = new ChartWindow(nullptr, "quat");

    connect(this, SIGNAL(sigUpdateBaseFormChart(id0x91_t)),
            this, SLOT(updateBaseFormChart(id0x91_t)), Qt::DirectConnection);


    //page 2 widget initialize : 3D widget
    ch_threeDform = new ThreeDForm();
    connect(this, SIGNAL(sigSendIMUtoThreeD(id0x91_t)),
            ch_threeDform, SLOT(getIMUPackets(id0x91_t)));



    //page 3
    ch_csvlogform = new CSVLogForm();

    connect(ch_serialport, SIGNAL(sigSendIMU(QVector<id0x91_t>)),
            ch_csvlogform, SLOT(getIMUPackets(QVector<id0x91_t>)));
    connect(ch_serialport, SIGNAL(sigSendBitmap(uchar)),
            ch_csvlogform, SLOT(getBitmap(uchar)));
    connect(ch_serialport, SIGNAL(sigPortClosed()),
            ch_csvlogform, SLOT(stopLogging()));

    //page 4
    ch_settingform = new CHSettingForm();

    connect(ch_settingform, SIGNAL(sigSetParam(char, uint32_t*, int16_t)),
            ch_serialport, SLOT(sltRWMdbus(char, uint32_t*, int16_t)));
    connect(ch_serialport, SIGNAL(sigMdbusParamLoaded()),
            ch_settingform, SLOT(sltMdbusParamLoaded()));

    connect(ch_settingform, SIGNAL(sigSendATcmd(QString)), ch_serialport, SLOT(slt_writeData(QString)));

    //about form
    m_aboutform = new AboutForm(this);
    m_aboutform->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    m_aboutform->setFixedSize(500, 500);


    //Welcome message
    statusbar_msg.baudrate = "";
    statusbar_msg.port = "";
    statusbar_msg.current_status = tr("Unconnected");
    statusbar_msg.sw_version = tr("Software Version : %1").arg(sf_version);
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());


    //set page 1 as the default page

    update_BTNConnect_state();

}

void BaseForm::closeEvent (QCloseEvent *event)
{

    ch_serialport->closePort();

    //serial port
    ch_comform->close();

    ch_settingform->close();
    ch_threeDform->close();
    ch_csvlogform->close();

    //charts
    m_chartAcc->close();
    m_chartGyr->close();
    m_chartMag->close();
    m_chartEul->close();
    m_chartQuat->close();

    m_aboutform->close();
    event->accept();

}
BaseForm::~BaseForm()
{
    delete ui;
}

///In SideBar///
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief BaseForm::on_SideBarBTN2/3/4_clicked
 * click to show pages
 */

void BaseForm::on_SideBarBTN2_clicked()
{
    ch_threeDform->show();
    ch_threeDform->startThreeDPlot();

}
void BaseForm::on_SideBarBTN3_clicked()
{
    ch_csvlogform->show();
}
void BaseForm::on_SideBarBTN4_clicked()
{
    ch_settingform->settingConfig_init();
    ch_settingform->show();

}


/**
 * @brief BaseForm::on_BTNConnect_clicked -
 * 1. serialport is not opened:show ch_comform
 * 2. serialport is opened:disconnect it
 * 3. while ch_comform is opened, BTN is disabled
 */
void BaseForm::on_BTNConnect_clicked()
{
    if(!ch_serialport->PortIsOpened()) {
        ch_comform->show();
        ch_comform->on_BTNPortRefresh_clicked();
    }

    update_BTNConnect_state();

    QVector<id0x91_t> a;
    updateDongleNodeList(false, a);
}

void BaseForm::on_BTNDisconnect_clicked()
{
    if(ch_serialport->PortIsOpened()) {
        ch_serialport->closePort();

        QVector<id0x91_t> a;
        updateDongleNodeList(false, a);
    }
}

void BaseForm::update_BTNConnect_state()
{
    bool isOpen = ch_serialport->PortIsOpened();

    if(!isOpen) {
        ui->BTNConnect->setFixedWidth(190);
        if(ch_comform->isVisible()) {
            ui->BTNConnect->setEnabled(false);
        } else {
            ui->BTNConnect->setEnabled(true);
            ui->BTNDisconnect->hide();
        }

    } else {
        ui->BTNConnect->setFixedWidth(150);
        ui->BTNConnect->setEnabled(false);
        ui->BTNDisconnect->show();
    }
    //enable all pages

    ch_threeDform->setEnabled(isOpen);
    ch_csvlogform->setEnabled(isOpen);
    ch_settingform->setEnabled(isOpen);
}

/**
 * @brief BaseForm::updateDongleNodeList
 * if number of nodes changes send sigUpdateDongleNodeList():call updateDongleNodeList()
 */
void BaseForm::updateDongleNodeList(bool is_dongle, QVector<id0x91_t> packets)
{

    if(is_dongle == 1) {
        ui->DongleNodeList->clear();
        ui->DongleNodeList->setVisible(true);

        int node_cnt_tmp = packets.length();

        if(node_cnt_tmp == 0) {
            ui->DongleNodeList->addItem(tr("No node is online."));

        } else {
            bool idexist = false;
            for(int i = 0; i < node_cnt_tmp; i++) {
                int t_id = packets.at(i).id;

                ui->DongleNodeList->addItem(tr("Wireless Node ID : %1").arg(t_id));

                if(t_id == cur_dongle_nodeID) {
                    ui->DongleNodeList->setCurrentRow(i);
                    cur_dongle_nodeIndex = i;
                    idexist = true;
                }

            }
            if(idexist == false)
                cur_dongle_nodeIndex = 0;
        }

    } else
        ui->DongleNodeList->setVisible(false);
}

/**
 * @brief BaseForm::on_DongleNodeList_itemClicked
 * @param cur_dongle_nodeID:remember the chosen ID
 */
void BaseForm::on_DongleNodeList_itemClicked(QListWidgetItem *item)
{
    QString id = ui->DongleNodeList->currentItem()->text().split(" : ").last();

    cur_dongle_nodeID = id.toInt();
    cur_dongle_nodeIndex = ui->DongleNodeList->currentRow();

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
    ch_serialport->linkCHdevices(port_name, baudrate);

    statusbar_msg.baudrate = QString::number(baudrate);
    statusbar_msg.port = port_name;
    statusbar_msg.current_status = tr("Connecting...");
    statusbar_msg.sw_version = "";
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
    showMessageBox(tr("Cannot build connection"), tr("Error"));

    ch_comform->show();

    statusbar_msg.current_status = tr("Cannot build connection. Please check the selected port again");
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());
    update_BTNConnect_state();

}
void BaseForm::getsigPortOpened()
{
    ch_comform->hide();

    //"connect" BTN
    update_BTNConnect_state();

    //statusbar
    statusbar_msg.current_status = tr("Streaming...");
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());
}

void BaseForm::getsigPortClosed()
{

    //close serial thread by main thread
    ch_serialport->quitmThread();

    //statusbar
    statusbar_msg.baudrate = "";
    statusbar_msg.port = "";
    statusbar_msg.current_status = tr("Unconnected");
    statusbar_msg.sw_version = tr("Software Version : %1").arg(sf_version);
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());

    update_BTNConnect_state();

}

/**
 * @brief BaseForm::getIMUPackets, BaseForm::getDongleData
 * process 2 kinds of data: hi226/hi229/ch110/hi221 vs hi221gw
 * @param cur_dongle_nodeIndex: while streaming hi221gw, it will remember the chose node(by ID) until it's offine
 */
void BaseForm::getIMUPackets(QVector<id0x91_t> packets)
{
    int num_imu=packets.length();

    mutex_writing.lock();

    if(num_imu==0){
        //no node, pass
    } else if(num_imu==1){
        m_imu_data=packets.first();
        emit sigUpdateBaseFormChart(m_imu_data);
        emit sigSendIMUtoThreeD(m_imu_data);

    } else if(num_imu>1){
        m_imu_data = packets.at(cur_dongle_nodeIndex);
        emit sigUpdateBaseFormChart(m_imu_data);
        emit sigSendIMUtoThreeD(m_imu_data);
    }

    mutex_writing.unlock();
}

void BaseForm::getBitmap(uchar bitmap)
{
    m_contentbits=bitmap;
}


/**
 * @brief BaseForm::updateBaseForm()
 * I use timer to update.
 */
void BaseForm::updateBaseForm()
{
    //    static u_int sample_counter=0;
    //    uint update_interval=ch_serialport->Frame_rate/30;
    //    update_interval=update_interval*2;
    //    if(update_interval<1)
    //        update_interval=1;

    //    if(sample_counter%update_interval==0){

    //        updateIMUTable(m_imu_data, m_contentbits, m_protocol_tag);
    //        m_ADI->setData(m_imu_data.eul[0],m_imu_data.eul[1]);
    //        m_Compass->setYaw(m_imu_data.eul[2]);
    //        m_protocol_tag=0;
    //    }

    //    sample_counter++;

    updateIMUTable(m_imu_data, m_contentbits);
    ui->GLWidgetADI->setData(m_imu_data.eul[0], m_imu_data.eul[1]);
    ui->GLWidgetCompass->setYaw(m_imu_data.eul[2]);
}


/**
 * @brief BaseForm::getIMUmsg
 * when go into setting mode, node will transmit ACHii instead of HEX data.
 * @param str:message from node, BAUD is a special case because user need to re-connect manually.
 */
void BaseForm::getIMUmsg(QString str)
{
    if(str.indexOf("BAUD") >= 0) {
        statusbar_msg.current_status = tr("Please restart the device to take effect, and connect with new Baudrate.");
        ui->LabelStatusMsg->setText(statusbar_msg.getMsg());
    }
    if(str == "Data decoded error.") {
        statusbar_msg.current_status = tr("Data decoded error. Check if the Baudrate correct.");
        ui->LabelStatusMsg->setText(statusbar_msg.getMsg());
    }
}




/**
 * @brief BaseForm::updateIMUTable
 * @param imu_data:package from a single node
 * @param content_bits:define what kinds of data can be showed
 */

void BaseForm::updateIMUTable(id0x91_t imu_data, uchar content_bits)
{

    QString setptl = "";
    ui->LabelFrameRate->setText(QString::number(ch_serialport->Frame_rate) + " Hz");

    if(content_bits & BIT_VALID_ID) {
        if(!ui->LabelID->isVisible())
            ui->LabelID->setVisible(true);
        ui->LabelID->setText("ID = " + QString::number(imu_data.id));
        setptl += "90,";
    } else {
        if(ui->LabelID->isVisible())
            ui->LabelID->setVisible(false);

    }
    if(content_bits & BIT_VALID_ACC) {
        if(!ui->LabelGPAcc->isVisible()) {
            ui->LabelGPAcc->setVisible(true);
        }
        ui->LabelAccX->setText(QString::number(imu_data.acc[0], 'f', 3));
        ui->LabelAccY->setText(QString::number(imu_data.acc[1], 'f', 3));
        ui->LabelAccZ->setText(QString::number(imu_data.acc[2], 'f', 3));
        setptl += "A0,";
    } else {
        if(ui->LabelGPAcc->isVisible()) {
            ui->LabelGPAcc->setVisible(false);
        }
    }
    if(content_bits & BIT_VALID_GYR) {
        if(!ui->LabelGPGyro->isVisible())
            ui->LabelGPGyro->setVisible(true);
        ui->LabelGyroX->setText(QString::number(imu_data.gyr[0], 'f', 3));
        ui->LabelGyroY->setText(QString::number(imu_data.gyr[1], 'f', 3));
        ui->LabelGyroZ->setText(QString::number(imu_data.gyr[2], 'f', 3));
        setptl += "B0,";
    } else {
        if(ui->LabelGPGyro->isVisible())
            ui->LabelGPGyro->setVisible(false);
    }
    if(content_bits & BIT_VALID_MAG) {
        if(!ui->LabelGPMag->isVisible())
            ui->LabelGPMag->setVisible(true);
        ui->LabelMagX->setText(QString::number(imu_data.mag[0], 'f', 0));
        ui->LabelMagY->setText(QString::number(imu_data.mag[1], 'f', 0));
        ui->LabelMagZ->setText(QString::number(imu_data.mag[2], 'f', 0));
        setptl += "C0,";
    } else {
        if(ui->LabelGPMag->isVisible())
            ui->LabelGPMag->setVisible(false);
    }
    if(content_bits & BIT_VALID_EUL) {
        if(!ui->LabelGPEuler->isVisible())
            ui->LabelGPEuler->setVisible(true);
        ui->LabelEulerX->setText(QString::number(imu_data.eul[0], 'f', 2));
        ui->LabelEulerY->setText(QString::number(imu_data.eul[1], 'f', 2));
        ui->LabelEulerZ->setText(QString::number(imu_data.eul[2], 'f', 2));
        setptl += "D0,";
    } else {
        if(ui->LabelGPEuler->isVisible())
            ui->LabelGPEuler->setVisible(false);
    }
    if(content_bits & BIT_VALID_QUAT) {
        if(!ui->LabelGPQuat->isVisible())
            ui->LabelGPQuat->setVisible(true);
        ui->LabelQuatW->setText(QString::number(imu_data.quat[0], 'f', 3));
        ui->LabelQuatX->setText(QString::number(imu_data.quat[1], 'f', 3));
        ui->LabelQuatY->setText(QString::number(imu_data.quat[2], 'f', 3));
        ui->LabelQuatZ->setText(QString::number(imu_data.quat[3], 'f', 3));
        setptl += "D1,";
    } else {
        if(ui->LabelGPQuat->isVisible())
            ui->LabelGPQuat->setVisible(false);
    }

    if(content_bits & BIT_VALID_TIME_STAMP) {
        setptl = "91";
    }
    //no more 0x63
    //    if(protocol_tag == KItemDongleRaw)
    //        setptl="63";

    if (content_bits & BIT_RF_DONGLE)
        setptl = "62";


    //delete that last comma
    int ret = setptl.lastIndexOf(',');
    if(ret != -1)
        setptl.remove(ret, 1);

    ui->LabelDataProtocol->setText(tr("Data Protocol = %1").arg(setptl));

}

void BaseForm::updateBaseFormChart(id0x91_t imu_data)
{

    if(m_contentbits & BIT_VALID_ACC) {
        m_chartAcc->updateLineData(imu_data.acc);
        m_chartAcc->framerate = ch_serialport->Frame_rate;
    } else {
        m_chartAcc->setVisible(false);
    }
    if(m_contentbits & BIT_VALID_GYR) {
        m_chartGyr->updateLineData(imu_data.gyr);
        m_chartGyr->framerate = ch_serialport->Frame_rate;
    } else {
        m_chartGyr->setVisible(false);
    }
    if(m_contentbits & BIT_VALID_MAG) {
        m_chartMag->updateLineData(imu_data.mag);
        m_chartMag->framerate = ch_serialport->Frame_rate;
    } else {
        m_chartMag->setVisible(false);
    }
    if(m_contentbits & BIT_VALID_EUL) {
        m_chartEul->updateLineData(imu_data.eul);
        m_chartEul->framerate = ch_serialport->Frame_rate;
    } else {
        m_chartEul->setVisible(false);
    }
    if(m_contentbits & BIT_VALID_QUAT) {
        m_chartQuat->updateLineData(imu_data.quat);
        m_chartQuat->framerate = ch_serialport->Frame_rate;
    } else {
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

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << "Language=tc" << "\n";
        file.close();

    } else {
        QTextStream stream(&file);
        stream << "Language=tc" << "\n";
        file.close();
    }
    showMessageBox(tr("Please restart software to activate new language."), tr("Information"));
}

void BaseForm::on_actionSimplified_Chinese_triggered()
{
    QFile file("CHCenter.ini");

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << "Language=cn" << "\n";
        file.close();
    } else {
        QTextStream stream(&file);
        stream << "Language=cn" << "\n";
        file.close();
    }
    showMessageBox(tr("Please restart software to activate new language."), tr("Information"));
}

void BaseForm::on_actionEnglish_triggered()
{
    QFile file("CHCenter.ini");

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << "Language=en" << "\n";
        file.close();
    } else {
        QTextStream stream(&file);
        stream << "Language=en" << "\n";
        file.close();
    }

    showMessageBox(tr("Please restart software to activate new language."), tr("Information"));

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
    if(!m_chartAcc->isVisible()) {
        m_chartAcc->setVisible(true);
        m_chartAcc->init();
    } else
        m_chartAcc->setVisible(false);
}

void BaseForm::on_BTNChartGyr_clicked()
{
    if(!m_chartGyr->isVisible()) {
        m_chartGyr->setVisible(true);
        m_chartGyr->init();
    } else
        m_chartGyr->setVisible(false);
}

void BaseForm::on_BTNChartMag_clicked()
{
    if(!m_chartMag->isVisible()) {
        m_chartMag->setVisible(true);
        m_chartMag->init();
    } else
        m_chartMag->setVisible(false);
}

void BaseForm::on_BTNChartEul_clicked()
{
    if(!m_chartEul->isVisible()) {
        m_chartEul->setVisible(true);
        m_chartEul->init();
    } else
        m_chartEul->setVisible(false);
}

void BaseForm::on_BTNChartQuat_clicked()
{
    if(!m_chartQuat->isVisible()) {
        m_chartQuat->setVisible(true);
        m_chartQuat->init();
    } else
        m_chartQuat->setVisible(false);
}

