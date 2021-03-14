#include "chsettingform.h"
#include "ui_chsettingform.h"

CHSettingForm::CHSettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHSettingForm)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Device Settings"));

    connect(this, SIGNAL(sigSendATcmd(QString)), this, SLOT(displayATcmd(QString)));


    settingConfig_init();
    on_AdvancedCheckBox_clicked(false);
}

CHSettingForm::~CHSettingForm()
{

    delete ui;
}

void CHSettingForm::closeEvent(QCloseEvent *event)
{

    if (event->spontaneous()) {
        settingConfig_leave();
    } else {
        QWidget::closeEvent(event);
    }

}




void CHSettingForm::settingConfig_init()
{
    ui->GB_OldPTL->setVisible(false);

    new_ch_config=CH_Config;
    CH_Config.Baud=0;
    CH_Config.Setptl="";
    CH_Config.ODR=-1;
    CH_Config.GWID=0;
    CH_Config.ID=1;
    CH_Config.Mode=-1;

    //read mdbus
    emit sigSetParam('r', m_modbus_param);


    ui->CB_Advanced->setChecked(false);
    ui->GB_Advanced->setVisible(false);

    if(CH_Config.Setptl.indexOf("90")>=0)
        ui->IDCheckBox->setChecked(1);
    else
        ui->IDCheckBox->setChecked(0);
    if(CH_Config.Setptl.indexOf("A0")>=0)
        ui->AccCheckBox->setChecked(1);
    else
        ui->AccCheckBox->setChecked(0);
    if(CH_Config.Setptl.indexOf("B0")>=0)
        ui->GyroCheckBox->setChecked(1);
    else
        ui->GyroCheckBox->setChecked(0);
    if(CH_Config.Setptl.indexOf("C0")>=0)
        ui->MagCheckBox->setChecked(1);
    else
        ui->MagCheckBox->setChecked(0);
    if(CH_Config.Setptl.indexOf("D0")>=0)
        ui->EulerCheckBox->setChecked(1);
    else
        ui->EulerCheckBox->setChecked(0);
    if(CH_Config.Setptl.indexOf("D1")>=0)
        ui->QuatCheckBox->setChecked(1);
    else
        ui->QuatCheckBox->setChecked(0);
    if(CH_Config.Setptl.indexOf("91")>=0)
        ui->IMUSOLCheckBox->setChecked(1);
    else
        ui->IMUSOLCheckBox->setChecked(0);

    //init radio btn
    ui->FR0BTN->setAutoExclusive(0);
    ui->FR1BTN->setAutoExclusive(0);
    ui->FR25BTN->setAutoExclusive(0);
    ui->FR50BTN->setAutoExclusive(0);
    ui->FR100BTN->setAutoExclusive(0);
    ui->FR400BTN->setAutoExclusive(0);
    ui->FR0BTN->setChecked(0);
    ui->FR1BTN->setChecked(0);
    ui->FR25BTN->setChecked(0);
    ui->FR50BTN->setChecked(0);
    ui->FR100BTN->setChecked(0);
    ui->FR400BTN->setChecked(0);
    ui->FR0BTN->setAutoExclusive(1);
    ui->FR1BTN->setAutoExclusive(1);
    ui->FR25BTN->setAutoExclusive(1);
    ui->FR50BTN->setAutoExclusive(1);
    ui->FR100BTN->setAutoExclusive(1);
    ui->FR400BTN->setAutoExclusive(1);


    if(CH_Config.ODR==0)
        ui->FR0BTN->setChecked(1);
    else if(CH_Config.ODR==1)
        ui->FR1BTN->setChecked(1);
    else if(CH_Config.ODR==25)
        ui->FR25BTN->setChecked(1);
    else if(CH_Config.ODR==50)
        ui->FR50BTN->setChecked(1);
    else if(CH_Config.ODR==100)
        ui->FR100BTN->setChecked(1);
    else if(CH_Config.ODR==400)
        ui->FR400BTN->setChecked(1);

    ui->IDInput->setValue(CH_Config.ID);
    ui->GWIDInput->setValue(CH_Config.GWID);

    ui->TerminalBox->clear();

    //delay 100ms to clear text
    QEventLoop eventloop;
    QTimer::singleShot(100, &eventloop, SLOT(quit()));
    eventloop.exec();
    ui->TB_Termial->clear();

    //identify product
    identifyProduct();

}

void CHSettingForm::identifyProduct()
{
    bool is221=CH_Config.Model=="HI221";
    bool is221dongle=CH_Config.Model=="HI221Dongle";

    ui->Label_GWID->setVisible(is221||is221dongle);
    ui->SB_GWID->setVisible(is221||is221dongle);

    ui->GB_OldPTL->setVisible(!(is221||is221dongle));
    ui->CB_PTL->setVisible(!(is221||is221dongle));
    ui->Label_PTL->setVisible(!(is221||is221dongle));

    ui->Label_Mode->setVisible(!is221dongle);
    ui->CB_Mode->setVisible(!is221dongle);

    ui->Label_GWFRQ->setVisible(is221dongle);
    ui->CB_GWFRQ->setVisible(is221dongle);

    ui->Label_MaxNodeSize->setVisible(is221dongle);
    ui->CB_MaxNodeSize->setVisible(is221dongle);




}


void CHSettingForm::settingConfig_leave()
{


    QMessageBox msgBox;
    msgBox.setText(tr("Do you want to save settings and restart module?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, tr("Yes"));
    msgBox.setButtonText(QMessageBox::No, tr("Don't save"));

    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
        on_RSTBTN_clicked();
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }


    emit sigSendATcmd("AT+EOUT=1\r\n");
}

void CHSettingForm::setTerminalBoxText(QString str)
{
    ui->TerminalBox->append(str);
    ui->TB_Termial->append(str);

}



///obselete///
void CHSettingForm::StreamATcmd()
{
    emit sigSendATcmd("AT+EOUT=1\r\n");
}

void CHSettingForm::on_InfoBTN_clicked()
{
    emit sigSendATcmd("AT+INFO=L\r\n");
}

void CHSettingForm::on_RestartBTN_clicked()
{
    emit sigSendATcmd("AT+RST\r\n");
}

void CHSettingForm::displayATcmd(QString ATcmd)
{
    ui->ATcmdInput->setText(ATcmd);
}



void CHSettingForm::on_sendATcmdBTN_clicked()
{
    emit sigSendATcmd(ui->ATcmdInput->text());
}

void CHSettingForm::on_clearBTN_clicked()
{
    ui->TerminalBox->clear();
}

void CHSettingForm::on_StopStreamBTN_clicked()
{
    emit sigSendATcmd("AT+EOUT=0\r\n");
}



void CHSettingForm::on_BRSetBTN_clicked()
{

    if(ui->BR9600BTN->isChecked()){
        emit sigSendATcmd("AT+BAUD=9600\r\n");
        new_ch_config.Baud=9600;
    }
    else if(ui->BR115200BTN->isChecked()){
        emit sigSendATcmd("AT+BAUD=115200\r\n");
        new_ch_config.Baud=115200;
    }
    else if(ui->BR460800BTN->isChecked()){
        emit sigSendATcmd("AT+BAUD=460800\r\n");
        new_ch_config.Baud=460800;
    }
    else if(ui->BR921600BTN->isChecked()){
        emit sigSendATcmd("AT+BAUD=921600\r\n");
        new_ch_config.Baud=921600;
    }
}

void CHSettingForm::on_FRSetBTN_clicked()
{
    if(ui->FR1BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=1\r\n");
        new_ch_config.ODR=1;
    }
    else if(ui->FR0BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=0\r\n");
        new_ch_config.ODR=0;
    }
    else if(ui->FR25BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=25\r\n");
        new_ch_config.ODR=25;
    }
    else if(ui->FR50BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=50\r\n");
        new_ch_config.ODR=50;
    }
    else if(ui->FR100BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=100\r\n");
        new_ch_config.ODR=100;
    }
    else if(ui->FR400BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=400\r\n");
        new_ch_config.ODR=400;
    }

}

void CHSettingForm::on_Mode6AxisBTN_clicked()
{
    emit sigSendATcmd("AT+MODE=0\r\n");
    new_ch_config.Mode=0;

}

void CHSettingForm::on_Mode9AxisBTN_clicked()
{
    emit sigSendATcmd("AT+MODE=1\r\n");
    new_ch_config.Mode=1;
}

void CHSettingForm::on_ProtocolSetBTN_clicked()
{
    QString setptl="AT+SETPTL=";

    if(ui->IDCheckBox->isChecked()){
        setptl+="90,";
    }
    if(ui->AccCheckBox->isChecked()){
        setptl+="A0,";
    }
    if(ui->GyroCheckBox->isChecked()){
        setptl+="B0,";
    }
    if(ui->MagCheckBox->isChecked()){
        setptl+="C0,";
    }
    if(ui->EulerCheckBox->isChecked()){
        setptl+="D0,";
    }
    if(ui->QuatCheckBox->isChecked()){
        setptl+="D1,";
    }
    if(ui->OldPTLCheckBox->isChecked())
    {
        setptl="AT+SETPTL=90,A0,B0,C0,D0,D1,";
    }

    if(ui->IMUSOLCheckBox->isChecked()){
        setptl="AT+SETPTL=91";
        ui->IDCheckBox->setChecked(0);
        ui->AccCheckBox->setChecked(0);
        ui->GyroCheckBox->setChecked(0);
        ui->MagCheckBox->setChecked(0);
        ui->EulerCheckBox->setChecked(0);
        ui->QuatCheckBox->setChecked(0);
    }

    //remove the last comma
    int ret=setptl.lastIndexOf(',');
    if(ret!=-1)
        setptl.remove(ret,1);
    new_ch_config.Setptl=setptl;
    setptl+="\r\n";
    emit sigSendATcmd(setptl);
}

void CHSettingForm::on_IMUSOLCheckBox_clicked()
{
    if(ui->IMUSOLCheckBox->isChecked()){
        ui->OldPTLCheckBox->setChecked(0);
        ui->IDCheckBox->setChecked(0);
        ui->AccCheckBox->setChecked(0);
        ui->GyroCheckBox->setChecked(0);
        ui->MagCheckBox->setChecked(0);
        ui->EulerCheckBox->setChecked(0);
        ui->QuatCheckBox->setChecked(0);
    }
}

void CHSettingForm::on_OldPTLCheckBox_clicked(bool checked)
{
    if(checked)
        ui->IMUSOLCheckBox->setChecked(false);

    ui->IDCheckBox->setChecked(checked);
    ui->AccCheckBox->setChecked(checked);
    ui->GyroCheckBox->setChecked(checked);
    ui->MagCheckBox->setChecked(checked);
    ui->EulerCheckBox->setChecked(checked);
    ui->QuatCheckBox->setChecked(checked);
}




void CHSettingForm::on_IDCheckBox_clicked()
{
    ui->IMUSOLCheckBox->setChecked(0);
    ui->OldPTLCheckBox->setChecked(0);
}

void CHSettingForm::on_AccCheckBox_clicked()
{
    ui->IMUSOLCheckBox->setChecked(0);
    ui->OldPTLCheckBox->setChecked(0);
}

void CHSettingForm::on_GyroCheckBox_clicked()
{
    ui->IMUSOLCheckBox->setChecked(0);
    ui->OldPTLCheckBox->setChecked(0);
}

void CHSettingForm::on_MagCheckBox_clicked()
{
    ui->IMUSOLCheckBox->setChecked(0);
    ui->OldPTLCheckBox->setChecked(0);
}

void CHSettingForm::on_EulerCheckBox_clicked()
{
    ui->IMUSOLCheckBox->setChecked(0);
    ui->OldPTLCheckBox->setChecked(0);
}

void CHSettingForm::on_QuatCheckBox_clicked()
{
    ui->IMUSOLCheckBox->setChecked(0);
    ui->OldPTLCheckBox->setChecked(0);
}

void CHSettingForm::on_IDSetBTN_clicked()
{
    emit sigSendATcmd("AT+ID="+ui->IDInput->text()+"\r\n");
    new_ch_config.ID=ui->IDInput->text().toInt();
}

void CHSettingForm::on_GWIDSetBTN_clicked()
{
    emit sigSendATcmd("AT+GWID="+ui->GWIDInput->text()+"\r\n");
    new_ch_config.GWID=ui->GWIDInput->text().toInt();
}



void CHSettingForm::on_AdvancedCheckBox_clicked(bool checked)
{
    if(checked){
        ui->BaudrateGB->setVisible(true);

        ui->OldPTLCheckBox->setVisible(false);

        ui->IDCheckBox->setVisible(true);
        ui->AccCheckBox->setVisible(true);
        ui->GyroCheckBox->setVisible(true);
        ui->MagCheckBox->setVisible(true);
        ui->EulerCheckBox->setVisible(true);
        ui->QuatCheckBox->setVisible(true);

    }
    else{
        ui->BaudrateGB->setVisible(false);

        ui->OldPTLCheckBox->setVisible(true);

        ui->IDCheckBox->setVisible(false);
        ui->AccCheckBox->setVisible(false);
        ui->GyroCheckBox->setVisible(false);
        ui->MagCheckBox->setVisible(false);
        ui->EulerCheckBox->setVisible(false);
        ui->QuatCheckBox->setVisible(false);

    }
}
///////obselete///////


void CHSettingForm::on_LoadParamBTN_clicked()
{

    //sigSetParam(read or write, array[], -1:write all, >0 write to rigister addr)
    emit sigSetParam('r', m_modbus_param);

}

void CHSettingForm::sltMdbusParamLoaded()
{

    //    for(int i=0; i<112;i++){
    //        qDebug()<<m_modbus_param[i];
    //    }


    //read model, UUID
    QString prod_info;

    switch(m_modbus_param[0] & 0xFFFF){
    case 100:
        CH_Config.Model=("CH100");
        break;
    case 110:
        CH_Config.Model=("CH110");
        break;
    case 221:
        CH_Config.Model=("HI221");
        break;
    case 222:
        CH_Config.Model=("HI221Dongle");
        break;
    case 226:
        CH_Config.Model=("HI226");
        break;
    case 229:
        CH_Config.Model=("HI229");
        break;
    default:
        CH_Config.Model=("Unrecognized");
        break;
    }


    prod_info.append(tr("Product: %1, ").arg(CH_Config.Model));
    prod_info.append(tr("UUID: %1%2 ").arg(m_modbus_param[2],0,16).arg(m_modbus_param[3],0,16));

    //read ID, RF parameters
    CH_Config.ID=m_modbus_param[4];
    CH_Config.GWID=m_modbus_param[40]& 0x000000FF;

    CH_Config.MaxNodeSize=(m_modbus_param[40]>>8)& 0x000000FF;
    CH_Config.GWFRQ=(m_modbus_param[40]>>16)& 0x000000FF;

    //read Baud
    CH_Config.Baud=m_modbus_param[9];

    //read bitmap
    CH_Config.Setptl="";
    CH_Config.Bitmap=uint16_t(m_modbus_param[10] & 0x0000FFFF);

    //read ODR
    CH_Config.ODR=uint16_t(m_modbus_param[10] >> 16);

    //read mode
    CH_Config.Mode=m_modbus_param[16];


    //UI load ProdInfo
    ui->Label_ProdInfo->setText(prod_info);

    //UI load ID, RF parameters
    ui->SB_ID->setValue(CH_Config.ID);
    ui->SB_GWID->setValue(CH_Config.GWID);

    for(int i=0;i < ui->CB_GWFRQ->count();i++){

        if(CH_Config.GWFRQ==ui->CB_GWFRQ->itemText(i).toUInt()){
            ui->CB_GWFRQ->setCurrentIndex(i);
        }
    }
    for(int i=0;i < ui->CB_MaxNodeSize->count();i++){

        if(CH_Config.MaxNodeSize==ui->CB_MaxNodeSize->itemText(i).toUInt()){
            ui->CB_MaxNodeSize->setCurrentIndex(i);
        }
    }

    //UI load Baud
    for(int i=0;i < ui->CB_Baud->count();i++){
        if(CH_Config.Baud==ui->CB_Baud->itemText(i).toUInt()){
            ui->CB_Baud->setCurrentIndex(i);
        }
    }

    //UI load ODR
    for(int i=0;i < ui->CB_ODR->count();i++){
        if(CH_Config.ODR==ui->CB_ODR->itemText(i).toUInt()){
            ui->CB_ODR->setCurrentIndex(i);
        }
    }
    //UI load mode

    if(CH_Config.Mode==0x01)
        ui->CB_Mode->setCurrentIndex(0); //6 axis
    else if(CH_Config.Mode==0x03)
        ui->CB_Mode->setCurrentIndex(1); //9 axis


    //UI load protocol
    uint16_t temp_Bitmap=0x0001;
    if(CH_Config.Bitmap&temp_Bitmap){
        CH_Config.Setptl="91";
        ui->CB_PTL->setCurrentIndex(0);
    }
    else if(CH_Config.Bitmap&(temp_Bitmap<<8)){
        CH_Config.Setptl="62";
    }
    else{
        ui->CB_PTL->setCurrentIndex(1);

        if((CH_Config.Bitmap&(temp_Bitmap<<1))){
            CH_Config.Setptl+="90,";
            ui->CB_90->setChecked(true);
        }
        if((CH_Config.Bitmap&(temp_Bitmap<<2))){
            CH_Config.Setptl+="A0,";
            ui->CB_A0->setChecked(true);
        }
        if((CH_Config.Bitmap&(temp_Bitmap<<3))){
            CH_Config.Setptl+="B0,";
            ui->CB_B0->setChecked(true);
        }
        if((CH_Config.Bitmap&(temp_Bitmap<<4))){
            CH_Config.Setptl+="C0,";
            ui->CB_C0->setChecked(true);
        }
        if((CH_Config.Bitmap&(temp_Bitmap<<5))){
            CH_Config.Setptl+="D0,";
            ui->CB_D0->setChecked(true);
        }
        if((CH_Config.Bitmap&(temp_Bitmap<<6))){
            CH_Config.Setptl+="D1,";
            ui->CB_D1->setChecked(true);
        }
        if((CH_Config.Bitmap&(temp_Bitmap<<7))){
            CH_Config.Setptl+="F0,";
            ui->CB_F0->setChecked(true);
        }


        int ret=CH_Config.Setptl.lastIndexOf(',');
        if(ret!=-1)
            CH_Config.Setptl.remove(ret,1);
    }

}




void CHSettingForm::on_BTN_clearTB_clicked()
{
    ui->TB_Termial->clear();
}

void CHSettingForm::on_CB_PTL_activated(int index)
{
    if(index){
        ui->GB_OldPTL->setVisible(true);
        ui->CB_90->setChecked(true);
        ui->CB_A0->setChecked(true);
        ui->CB_B0->setChecked(true);
        ui->CB_C0->setChecked(true);
        ui->CB_D0->setChecked(true);
        ui->CB_D1->setChecked(true);
        ui->CB_F0->setChecked(true);
        writeUART_CFG();
    }
    else{
        ui->GB_OldPTL->setVisible(false);
        writeUART_CFG();
    }
}



void CHSettingForm::on_CB_90_clicked()
{
    writeUART_CFG();
}
void CHSettingForm::on_CB_A0_clicked()
{
    writeUART_CFG();
}
void CHSettingForm::on_CB_B0_clicked()
{
    writeUART_CFG();
}
void CHSettingForm::on_CB_C0_clicked()
{
    writeUART_CFG();
}
void CHSettingForm::on_CB_D0_clicked()
{
    writeUART_CFG();
}
void CHSettingForm::on_CB_D1_clicked()
{
    writeUART_CFG();
}
void CHSettingForm::on_CB_F0_clicked()
{
    writeUART_CFG();
}


void CHSettingForm::on_CB_ODR_activated(const QString &arg1)
{

    CH_Config.ODR=arg1.toUInt();
    writeUART_CFG();
}

void CHSettingForm::on_CB_Mode_activated(int index)
{
    if(index==0)
        CH_Config.Mode=0x01;
    else if(index==1)
        CH_Config.Mode=0x03;
    writeMode();
}

void CHSettingForm::on_CB_Baud_activated(const QString &arg1)
{
    qDebug()<<"Baud_currentIndexChanged";
    CH_Config.Baud=arg1.toUInt();
    writeBaud();
}
void CHSettingForm::on_BTN_ATCMD_clicked()
{
    emit sigSendATcmd(ui->LE_ATCMD->text()+"\r\n");
}

void CHSettingForm::on_CB_Advanced_stateChanged(int arg1)
{
    ui->GB_Advanced->setVisible(arg1);
}
void CHSettingForm::writeUART_CFG()
{
    if(CH_Config.Model=="HI221"){
        CH_Config.Bitmap=0x0001;
    }

    else if(CH_Config.Model=="HI221Dongle"){

        CH_Config.Bitmap=0x0001<<8;
    }
    else{
        if(ui->CB_PTL->currentIndex()==0){
            CH_Config.Bitmap=0x0001;
        }
        else{
            uint16_t temp_bitmap=0;

            if(ui->CB_F0->isChecked())
                temp_bitmap++;
            temp_bitmap=(temp_bitmap<<1);
            if(ui->CB_D1->isChecked())
                temp_bitmap++;
            temp_bitmap=(temp_bitmap<<1);
            if(ui->CB_D0->isChecked())
                temp_bitmap++;
            temp_bitmap=(temp_bitmap<<1);
            if(ui->CB_C0->isChecked())
                temp_bitmap++;
            temp_bitmap=(temp_bitmap<<1);
            if(ui->CB_B0->isChecked())
                temp_bitmap++;
            temp_bitmap=(temp_bitmap<<1);
            if(ui->CB_A0->isChecked())
                temp_bitmap++;
            temp_bitmap=(temp_bitmap<<1);
            if(ui->CB_90->isChecked())
                temp_bitmap++;
            temp_bitmap=(temp_bitmap<<1);

            CH_Config.Bitmap=temp_bitmap;
        }
    }
    //new ODR
    CH_Config.ODR=uint16_t(ui->CB_ODR->currentText().toUInt());
    uint32_t temp_UART_CFG=(CH_Config.ODR<<16)+CH_Config.Bitmap;
    m_modbus_param[10]=temp_UART_CFG;

    emit sigSetParam('w', &m_modbus_param[10], 10);

}
void CHSettingForm::writeMode()
{
    emit sigSetParam('w', &CH_Config.Mode, 16);
}
void CHSettingForm::writeBaud()
{
    emit sigSetParam('w', &CH_Config.Baud, 9);
}


void CHSettingForm::on_RSTBTN_clicked()
{
    uint32_t tmp_cmd_flash=0x06; //save all parameters to flash

    m_modbus_param[17]=tmp_cmd_flash;
    emit sigSetParam('w', &m_modbus_param[17], 17); //17:CMD

    QTimer::singleShot(200, this, SLOT(writeRST()));

}
void CHSettingForm::writeRST()
{
    uint32_t tmp_cmd_rst=0x05;  //RST
    m_modbus_param[17]=tmp_cmd_rst;
    emit sigSetParam('w', &m_modbus_param[17], 17);
}

void CHSettingForm::on_BTN_PrintCalib_clicked()
{
    QString text;
    text="ACC Calibration Parameters:\n";
    for(int i = 67;i<79;i++){
        text+=tr("%1 ").arg(QString::number(*(float*)&m_modbus_param[i],'f',4),20);
        if((i-67)%3==2){
            text+='\n';
        }

    }
    setTerminalBoxText(text);
}

void CHSettingForm::on_CB_MaxNodeSize_activated(const QString &arg1)
{

    CH_Config.MaxNodeSize=arg1.toUInt();

    uint32_t tmp_rf=0;
    tmp_rf+=CH_Config.GWFRQ;
    tmp_rf=tmp_rf<<8;
    tmp_rf+=CH_Config.MaxNodeSize;
    tmp_rf=tmp_rf<<8;
    tmp_rf+=CH_Config.GWID;

    m_modbus_param[40]=tmp_rf;

    emit sigSetParam('w', &m_modbus_param[40], 40);

}

void CHSettingForm::on_CB_GWFRQ_activated(const QString &arg1)
{
    CH_Config.GWFRQ=arg1.toUInt();

    uint32_t tmp_rf=0;
    tmp_rf+=CH_Config.GWFRQ;
    tmp_rf=tmp_rf<<8;
    tmp_rf+=CH_Config.MaxNodeSize;
    tmp_rf=tmp_rf<<8;
    tmp_rf+=CH_Config.GWID;

    m_modbus_param[40]=tmp_rf;

    emit sigSetParam('w', &m_modbus_param[40], 40);
}

void CHSettingForm::on_SB_GWID_valueChanged(int arg1)
{
    CH_Config.GWID=arg1;

    uint32_t tmp_rf=0;
    tmp_rf+=CH_Config.GWFRQ;
    tmp_rf=tmp_rf<<8;
    tmp_rf+=CH_Config.MaxNodeSize;
    tmp_rf=tmp_rf<<8;
    tmp_rf+=CH_Config.GWID;

    m_modbus_param[40]=tmp_rf;

    emit sigSetParam('w', &m_modbus_param[40], 40);
}

void CHSettingForm::on_SB_ID_valueChanged(int arg1)
{
    CH_Config.ID=arg1;
    emit sigSetParam('w', &CH_Config.ID, 4);
}


