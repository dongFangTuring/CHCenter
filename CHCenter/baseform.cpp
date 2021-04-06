#include "baseform.h"
#include "ui_baseform.h"

/**
 * @brief BaseForm::BaseForm -
 * set stylesheet, signals, ui when opening
 *
 *
 */

QString sf_version = "1.2.0"; //major.minor.bugix

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
    ui->centralwidget->setStyleSheet("background-color:rgb(250, 250, 250);");


    //initial the HI221GW node choosing widget
    ui->DongleNodeList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->DongleNodeList->setVisible(false);


    ch_comform = new form_com_slect(this);
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
    m_aboutform->setLabelVersion(sf_version);


    //Welcome message
    statusbar_msg.baudrate = "";
    statusbar_msg.port = "";
    statusbar_msg.current_status = tr("Unconnected");
    statusbar_msg.sw_version = tr("Software Version : %1").arg(sf_version);
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());
}

void BaseForm::closeEvent (QCloseEvent *event)
{

    ch_serialport->closePort();


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

/**
 * @brief BaseForm::on_BTNConnect_clicked -
 * 1. serialport is not opened:show ch_comform
 * 2. serialport is opened:disconnect it
 * 3. while ch_comform is opened, BTN is disabled
 */
void BaseForm::on_actionSerial_Port_triggered()
{
    if(!ch_serialport->isOpen())
    {
        if (ch_comform->exec() == QDialog::Accepted)
        {
            ch_serialport->linkCHdevices(ch_comform->port_name, ch_comform->baud);

            statusbar_msg.baudrate = QString::number(ch_comform->baud);
            statusbar_msg.port = ch_comform->port_name;
            statusbar_msg.current_status = tr("Connecting...");
            statusbar_msg.sw_version = "";
            ui->LabelStatusMsg->setText(statusbar_msg.getMsg());

            QVector<id0x91_t> a;
            updateDongleNodeList(false, a);
        }


    }
}

void BaseForm::on_actionStop_Connection_triggered()
{
    if(ch_serialport->isOpen()) {
        ch_serialport->closePort();

        QVector<id0x91_t> a;
        updateDongleNodeList(false, a);
    }
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
    Q_UNUSED(item);
    QString id = ui->DongleNodeList->currentItem()->text().split(" : ").last();

    cur_dongle_nodeID = id.toInt();
    cur_dongle_nodeIndex = ui->DongleNodeList->currentRow();
}



///signal from chserial class///
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

void BaseForm::geterrorOpenPort()
{
    showMessageBox(tr("Cannot build connection"), tr("Error"));
    statusbar_msg.current_status = tr("Cannot build connection. Please check the selected port again");
    ui->LabelStatusMsg->setText(statusbar_msg.getMsg());

}
void BaseForm::getsigPortOpened()
{
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

    if(num_imu==0) {
        //no node, pass
    } else if(num_imu==1) {
        m_imu_data=packets.first();
        emit sigUpdateBaseFormChart(m_imu_data);
        emit sigSendIMUtoThreeD(m_imu_data);

    } else if(num_imu>1) {
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
    if(str == "DECODE_ERR") {
        statusbar_msg.current_status = tr("Data decoded error. Check if the Baudrate correct.");
        ui->LabelStatusMsg->setText(statusbar_msg.getMsg());
    } else {
        ch_settingform->appendText(str);

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
        ui->LabelID->setText(tr("ID = %1").arg(imu_data.id));
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
        if(!ui->LabelGPGyro->isVisible()) {
            ui->LabelGPGyro->setVisible(true);
        }
        ui->LabelGyroX->setText(QString::number(imu_data.gyr[0], 'f', 3));
        ui->LabelGyroY->setText(QString::number(imu_data.gyr[1], 'f', 3));
        ui->LabelGyroZ->setText(QString::number(imu_data.gyr[2], 'f', 3));
        setptl += "B0,";
    } else {
        if(ui->LabelGPGyro->isVisible()) {
            ui->LabelGPGyro->setVisible(false);
        }
    }
    if(content_bits & BIT_VALID_MAG) {
        if(!ui->LabelGPMag->isVisible()) {
            ui->LabelGPMag->setVisible(true);
        }

        ui->LabelMagX->setText(QString::number(imu_data.mag[0], 'f', 0));
        ui->LabelMagY->setText(QString::number(imu_data.mag[1], 'f', 0));
        ui->LabelMagZ->setText(QString::number(imu_data.mag[2], 'f', 0));
        setptl += "C0,";
    } else {
        if(ui->LabelGPMag->isVisible()) {
            ui->LabelGPMag->setVisible(false);
        }
    }
    if(content_bits & BIT_VALID_EUL) {
        if(!ui->LabelGPEuler->isVisible()) {
            ui->LabelGPEuler->setVisible(true);
        }
        ui->LabelEulerX->setText(QString::number(imu_data.eul[0], 'f', 2));
        ui->LabelEulerY->setText(QString::number(imu_data.eul[1], 'f', 2));
        ui->LabelEulerZ->setText(QString::number(imu_data.eul[2], 'f', 2));
        setptl += "D0,";
    } else {
        if(ui->LabelGPEuler->isVisible()) {
            ui->LabelGPEuler->setVisible(false);
        }
    }
    if(content_bits & BIT_VALID_QUAT) {
        if(!ui->LabelGPQuat->isVisible()) {
            ui->LabelGPQuat->setVisible(true);
        }
        ui->LabelQuatW->setText(QString::number(imu_data.quat[0], 'f', 3));
        ui->LabelQuatX->setText(QString::number(imu_data.quat[1], 'f', 3));
        ui->LabelQuatY->setText(QString::number(imu_data.quat[2], 'f', 3));
        ui->LabelQuatZ->setText(QString::number(imu_data.quat[3], 'f', 3));
        setptl += "D1,";
    } else {
        if(ui->LabelGPQuat->isVisible()) {
            ui->LabelGPQuat->setVisible(false);
        }
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
        m_chartAcc->setFrameRate(ch_serialport->Frame_rate);
    }
    if(m_contentbits & BIT_VALID_GYR) {
        m_chartGyr->updateLineData(imu_data.gyr);
        m_chartGyr->setFrameRate(ch_serialport->Frame_rate);
    }
    if(m_contentbits & BIT_VALID_MAG) {
        m_chartMag->updateLineData(imu_data.mag);
        m_chartMag->setFrameRate(ch_serialport->Frame_rate);
    }
    if(m_contentbits & BIT_VALID_EUL) {
        m_chartEul->updateLineData(imu_data.eul);
        m_chartEul->setFrameRate(ch_serialport->Frame_rate);
    }
    if(m_contentbits & BIT_VALID_QUAT) {
        m_chartQuat->updateLineData(imu_data.quat);
        m_chartQuat->setFrameRate(ch_serialport->Frame_rate);
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



///tools in menubar///

void BaseForm::on_action3D_view_triggered()
{
    ch_threeDform->show();
}

void BaseForm::on_actionCSV_Logger_triggered()
{
    ch_csvlogform->show();
}

void BaseForm::on_actionDevice_Settiing_triggered()
{
    ch_settingform->show();
}


///chart actions in menubar///
void BaseForm::on_actionLinear_Acceleration_triggered()
{
    m_chartAcc->show();
}

void BaseForm::on_actionGyroscope_triggered()
{
    m_chartGyr->show();
}

void BaseForm::on_actionMagnetometer_triggered()
{
    m_chartMag->show();
}

void BaseForm::on_actionEuler_Angle_triggered()
{
    m_chartEul->show();
}

void BaseForm::on_actionQuaternion_triggered()
{
    m_chartQuat->show();
}


void BaseForm::on_actionWebside_triggered()
{
    QDesktopServices::openUrl(QUrl("https://hipnuc.com/datahub.html"));
}
