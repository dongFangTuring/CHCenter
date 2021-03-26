#include "chsettingform.h"
#include "ui_chsettingform.h"

CHSettingForm::CHSettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHSettingForm)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Device Settings"));

    settingConfig_init();

}

CHSettingForm::~CHSettingForm()
{

    delete ui;
}

void CHSettingForm::appendText(QString line)
{
    ui->TB_Termial->append(line);
}

void CHSettingForm::closeEvent(QCloseEvent *event)
{

    if (event->spontaneous()) {
        writeCmd(0x08); /* ENABLE OUTPUT */
    } else {
        QWidget::closeEvent(event);
    }

}




void CHSettingForm::settingConfig_init()
{
    ui->GB_OldPTL->setVisible(false);

    CH_Config.Baud = 0;
    CH_Config.Setptl = "";
    CH_Config.ODR = 0;
    CH_Config.GWID = 0;
    CH_Config.ID = 0;
    CH_Config.Mode = 0;

    //read mdbus
    on_LoadParamBTN_clicked();
    //emit sigSetParam('r', m_modbus_param);


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
    bool is221 = CH_Config.Model == "HI221";
    bool is221dongle = CH_Config.Model == "HI221Dongle";

    ui->Label_ID->setVisible(is221);
    ui->SB_ID->setVisible(is221);

    ui->Label_GWID->setVisible(is221||is221dongle);
    ui->SB_GWID->setVisible(is221||is221dongle);

    ui->CB_PTL->setVisible(!(is221||is221dongle));
    ui->Label_PTL->setVisible(!(is221||is221dongle));

    ui->Label_Mode->setVisible(!is221dongle);
    ui->CB_Mode->setVisible(!is221dongle);

    ui->Label_DongleParam->setVisible(is221dongle);
    ui->CB_DongleParam->setVisible(is221dongle);


}



///obselete///
void CHSettingForm::StreamATcmd()
{
    emit sigSendATcmd("AT+EOUT=1\r\n");
}


///////obselete///////


void CHSettingForm::on_LoadParamBTN_clicked()
{

    //sigSetParam(read or write, array[], -1:write all, >0 write to rigister addr)
    QVector<int16_t> reg_toread={0,2,3,4,9,16,40};

    foreach(int16_t i,reg_toread){
        emit sigSetParam('r', &m_modbus_param[i],i);
        delay(30);
    }
    sltMdbusParamLoaded();
}

void CHSettingForm::sltMdbusParamLoaded()
{

    //    for(int i=0; i<112;i++){
    //        qDebug()<<m_modbus_param[i];
    //    }


    //read model, UUID
    QString prod_info;

    switch(m_modbus_param[0] & 0xFFFF) {
    case 100:
        CH_Config.Model = ("CH100");
        break;
    case 110:
        CH_Config.Model = ("CH110");
        break;
    case 221:
        CH_Config.Model = ("HI221");
        break;
    case 222:
        CH_Config.Model = ("HI221Dongle");
        break;
    case 226:
        CH_Config.Model = ("HI226");
        break;
    case 229:
        CH_Config.Model = ("HI229");
        break;
    default:
        CH_Config.Model = ("Unrecognized");
        break;
    }


    prod_info.append(tr("%1").arg(CH_Config.Model));
    prod_info.append(tr(" UUID: %1%2").arg(m_modbus_param[2], 0, 16).arg(m_modbus_param[3], 0, 16));

    //read ID, RF parameters
    CH_Config.ID = m_modbus_param[4];
    CH_Config.GWID = m_modbus_param[40] & 0xFF;

    CH_Config.MaxNodeSize = (m_modbus_param[40]>>8) & 0xFF;
    CH_Config.GWFRQ = (m_modbus_param[40]>>16) & 0xFF;

    //read Baud
    CH_Config.Baud = m_modbus_param[9];

    //read bitmap
    CH_Config.Setptl = "";
    CH_Config.Bitmap = uint16_t(m_modbus_param[10] & 0xFFFF);

    //read ODR
    CH_Config.ODR = uint16_t(m_modbus_param[10] >> 16);

    //read mode
    CH_Config.Mode = m_modbus_param[16];


    //UI load ProdInfo
    ui->Label_ProdInfo->setText(prod_info);

    //UI load ID, RF parameters
    ui->SB_ID->setValue(CH_Config.ID);
    ui->SB_GWID->setValue(CH_Config.GWID);

    switch (CH_Config.MaxNodeSize) {
    case 4:
        ui->CB_DongleParam->setCurrentIndex(0);
        break;
    case 8:
        ui->CB_DongleParam->setCurrentIndex(1);
        break;
    case 16:
        ui->CB_DongleParam->setCurrentIndex(2);
        break;
    default:
        qDebug()<<tr("Exception: dongle param-%1,%2").arg(CH_Config.MaxNodeSize).arg(CH_Config.GWFRQ);
        ui->CB_DongleParam->setCurrentIndex(1);
        break;

    }


    //UI load Baud
    for(int i=0; i < ui->CB_Baud->count(); i++) {
        if(CH_Config.Baud == ui->CB_Baud->itemText(i).toUInt()) {
            ui->CB_Baud->setCurrentIndex(i);
        }
    }

    //UI load ODR
    for(int i=0; i < ui->CB_ODR->count(); i++) {
        if(CH_Config.ODR == ui->CB_ODR->itemText(i).toUInt()) {
            ui->CB_ODR->setCurrentIndex(i);
        }
    }
    //UI load mode

    if(CH_Config.Mode == 0x01)
        ui->CB_Mode->setCurrentIndex(0); //6 axis
    else if(CH_Config.Mode == 0x03)
        ui->CB_Mode->setCurrentIndex(1); //9 axis


    //UI load protocol
    uint16_t temp_Bitmap = 0x0001;
    if(CH_Config.Bitmap&temp_Bitmap) {
        CH_Config.Setptl = "91";
        ui->CB_PTL->setCurrentIndex(0);
    } else if(CH_Config.Bitmap&(temp_Bitmap<<8)) {
        CH_Config.Setptl = "62";
    } else {
        ui->CB_PTL->setCurrentIndex(1);

        if((CH_Config.Bitmap&(temp_Bitmap<<1))) {
            CH_Config.Setptl+="90,";
            ui->CB_90->setChecked(true);
        }
        if((CH_Config.Bitmap&(temp_Bitmap<<2))) {
            CH_Config.Setptl+="A0,";
            ui->CB_A0->setChecked(true);
        }
        if((CH_Config.Bitmap&(temp_Bitmap<<3))) {
            CH_Config.Setptl+="B0,";
            ui->CB_B0->setChecked(true);
        }
        if((CH_Config.Bitmap&(temp_Bitmap<<4))) {
            CH_Config.Setptl+="C0,";
            ui->CB_C0->setChecked(true);
        }
        if((CH_Config.Bitmap&(temp_Bitmap<<5))) {
            CH_Config.Setptl+="D0,";
            ui->CB_D0->setChecked(true);
        }
        if((CH_Config.Bitmap&(temp_Bitmap<<6))) {
            CH_Config.Setptl+="D1,";
            ui->CB_D1->setChecked(true);
        }
        if((CH_Config.Bitmap&(temp_Bitmap<<7))) {
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
    if(index) {
        ui->GB_OldPTL->setVisible(true);
        ui->CB_90->setChecked(true);
        ui->CB_A0->setChecked(true);
        ui->CB_B0->setChecked(true);
        ui->CB_C0->setChecked(true);
        ui->CB_D0->setChecked(true);
        ui->CB_D1->setChecked(true);
        ui->CB_F0->setChecked(true);
        writeUART_CFG();
    } else {
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
}

void CHSettingForm::on_CB_Baud_activated(const QString &arg1)
{
    qDebug()<<"Baud_currentIndexChanged";
    CH_Config.Baud=arg1.toUInt();
}
void CHSettingForm::on_BTN_ATCMD_clicked()
{
    emit sigSendATcmd(ui->LE_ATCMD->text()+"\r\n");
}


void CHSettingForm::writeUART_CFG()
{
    if(CH_Config.Model=="HI221") {
        CH_Config.Bitmap=0x0001;
    }

    else if(CH_Config.Model=="HI221Dongle") {

        CH_Config.Bitmap=0x0001<<8;
    } else {
        if(ui->CB_PTL->currentIndex()==0) {
            CH_Config.Bitmap=0x0001;
        } else {
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


}


void CHSettingForm::delay(uint32_t ms)
{
    QEventLoop eventloop;
    QTimer::singleShot(ms, &eventloop, SLOT(quit()));
    eventloop.exec();
}



void CHSettingForm::on_RSTBTN_clicked()
{
    emit sigSetParam('w', &CH_Config.Baud, 9);       //baud
    delay(10);
    emit sigSetParam('w', &CH_Config.Mode, 16);      //mode
    delay(10);
    emit sigSetParam('w', &m_modbus_param[10], 10);  //ODR&bitmap
    delay(10);
    emit sigSetParam('w', &m_modbus_param[40], 40);  //RF parameters
    delay(10);
    emit sigSetParam('w', &CH_Config.ID, 4);         //ID
    delay(10);

    writeCmd(0x06); /* SAVE TO FLASH */

    uint8_t cmd = 0x05;  /* RST */
    QTimer::singleShot(1000, this, [&,cmd]() {
        writeCmd(cmd);
    });
}

void CHSettingForm::writeCmd(uint8_t cmd)
{
    m_modbus_param[17]= cmd;
    emit sigSetParam('w', &m_modbus_param[17], 17);
}


//void CHSettingForm::on_BTN_PrintCalib_clicked()
//{
//    QString text;
//    text="ACC Calibration Parameters:\n";
//    for(int i = 67; i<79; i++) {
//        text+=tr("%1 ").arg(QString::number(*(float*)&m_modbus_param[i],'f',4),20);
//        if((i-67)%3==2) {
//            text+='\n';
//        }

//    }
//    ui->TB_Termial->append(text);
//}




void CHSettingForm::on_CB_DongleParam_activated(int index)
{
    switch (index) {

    case 0:
        CH_Config.GWFRQ = 400;
        CH_Config.MaxNodeSize=4;
        break;
    case 1:
        CH_Config.GWFRQ = 200;
        CH_Config.MaxNodeSize=8;
        break;
    case 2:
        CH_Config.GWFRQ = 100;
        CH_Config.MaxNodeSize=16;
        break;
    default:
        break;


    }

    uint32_t tmp_rf=0;
    tmp_rf+=CH_Config.GWFRQ;
    tmp_rf=tmp_rf<<8;
    tmp_rf+=CH_Config.MaxNodeSize;
    tmp_rf=tmp_rf<<8;
    tmp_rf+=CH_Config.GWID;

    m_modbus_param[40]=tmp_rf;
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

}

void CHSettingForm::on_SB_ID_valueChanged(int arg1)
{
    CH_Config.ID=arg1;
}




