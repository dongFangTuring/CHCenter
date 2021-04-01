#include "chsettingform.h"
#include "ui_chsettingform.h"
#include <QAbstractButton>
CHSettingForm::CHSettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHSettingForm)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Device Settings"));

    //in cmd box press enter = click atcmd btn
    connect(ui->LE_ATCMD,SIGNAL(returnPressed()),ui->BTN_ATCMD, SLOT(click()));
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

void CHSettingForm::showEvent(QShowEvent *event)
{
    settingConfig_init();
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
    bool is226 = CH_Config.Model == "HI226";
    bool is221 = CH_Config.Model == "HI221";
    bool is221dongle = CH_Config.Model == "HI221Dongle";

    ui->Label_Mode->setEnabled(!is226);
    ui->CB_Mode->setEnabled(!is226);

    ui->Label_ID->setEnabled(is221);
    ui->SB_ID->setEnabled(is221);

    ui->Label_GWID->setEnabled(is221||is221dongle);
    ui->SB_GWID->setEnabled(is221||is221dongle);

    ui->CB_PTL->setEnabled(!(is221||is221dongle));
    ui->Label_PTL->setEnabled(!(is221||is221dongle));

    ui->Label_Mode->setEnabled(!is221dongle);
    ui->CB_Mode->setEnabled(!is221dongle);

    ui->Label_DongleParam->setEnabled(is221dongle);
    ui->CB_DongleParam->setEnabled(is221dongle);
}

void CHSettingForm::uiLoadConfig()
{
    //UI load ProdInfo
    ui->Label_ProdInfo->setText(CH_Config.Model+" UUID:"+CH_Config.UUID);

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


void CHSettingForm::on_LoadParamBTN_clicked()
{
    ui->LoadParamBTN->setEnabled(false);
    //sigSetParam(read or write, array[], -1:write all / >0 write to rigister addr)
    QVector<int16_t> reg_toread={0,2,3,4,9,10,16,40};

    uint32_t read_reg;

    foreach(int16_t i,reg_toread){
        emit sigSetParam('r', &read_reg,i);
        delay(40);
        sltMdbusParamLoaded(read_reg,i);

    }
    uiLoadConfig();

    ui->LoadParamBTN->setEnabled(true);
}

void CHSettingForm::sltMdbusParamLoaded(uint read_reg, uint reg_index)
{
    switch (reg_index) {
    case 0:{
        //read model, UUID
        switch(read_reg & 0xFFFF) {
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

        break;
    }
    case 2:{
        CH_Config.UUID.clear();
        CH_Config.UUID.append(tr("%1").arg(read_reg, 0, 16));
        break;
    }

    case 3:{
        CH_Config.UUID.append(tr("%1").arg(read_reg, 0, 16));
        break;
    }
    case 4:{
        //read ID, RF parameters
        CH_Config.ID = read_reg;
        break;
    }
    case 9:{
        //read Baud
        CH_Config.Baud = read_reg;
        break;
    }
    case 10:{
        //read bitmap
        CH_Config.Setptl = "";
        CH_Config.Bitmap = uint16_t(read_reg & 0xFFFF);

        //read ODR
        CH_Config.ODR = uint16_t(read_reg >> 16);
        break;
    }
    case 16:{
        //read mode
        CH_Config.Mode = read_reg;
        break;
    }
    case 40:{
        CH_Config.GWID = read_reg & 0xFF;
        CH_Config.MaxNodeSize = (read_reg>>8) & 0xFF;
        CH_Config.GWFRQ = (read_reg>>16) & 0xFF;
        break;
    }
    default:{

        break;
    }
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
    } else {
        ui->GB_OldPTL->setVisible(false);
    }
}

void CHSettingForm::on_BTN_ATCMD_clicked()
{
    emit sigSendATcmd(ui->LE_ATCMD->text()+"\r\n");
}


uint32_t CHSettingForm::calcu_UART_CFG()
{
    if(CH_Config.Model=="HI221") {
        CH_Config.Bitmap=0x0001;
    } else if(CH_Config.Model=="HI221Dongle") {
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
    return temp_UART_CFG;
}


void CHSettingForm::delay(uint32_t ms)
{
    QEventLoop eventloop;
    QTimer::singleShot(ms, &eventloop, SLOT(quit()));
    eventloop.exec();
}


void CHSettingForm::on_RSTBTN_clicked()
{
    ui->RSTBTN->setEnabled(false);
    ui->LoadParamBTN->setEnabled(false);

    //read mode from ui
    int index_mode=ui->CB_Mode->currentIndex();
    if(index_mode==0)
        CH_Config.Mode=0x01;
    else if(index_mode==1)
        CH_Config.Mode=0x03;

    //read baud from ui
    CH_Config.Baud=ui->CB_Baud->currentText().toUInt();

    emit sigSetParam('w', &CH_Config.Baud, 9);       //baud
    delay(15);
    emit sigSetParam('w', &CH_Config.Mode, 16);      //mode
    delay(15);

    tmp_cmd=calcu_UART_CFG();
    emit sigSetParam('w', &tmp_cmd, 10);  //ODR&bitmap
    delay(15);

    tmp_cmd=calcu_DongleParam();
    emit sigSetParam('w', &tmp_cmd, 40);  //RF parameters
    delay(15);

    if(CH_Config.Model=="HI221"){
        emit sigSetParam('w', &CH_Config.ID, 4);         //ID
        delay(15);
    }

    writeCmd(0x06); /* SAVE TO FLASH */
    delay(15);

    uint8_t cmd = 0x05;  /* RST */
    QTimer::singleShot(1000, this, [&,cmd]() {
        writeCmd(cmd);
        ui->RSTBTN->setEnabled(true);
        ui->LoadParamBTN->setEnabled(true);
    });


}

void CHSettingForm::writeCmd(uint8_t cmd)
{
    tmp_cmd=cmd;
    emit sigSetParam('w', &tmp_cmd, 17);
}

uint32_t CHSettingForm::calcu_DongleParam()
{

    int index=ui->CB_DongleParam->currentIndex();
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
    CH_Config.GWID=ui->SB_GWID->value();


    uint32_t tmp_rf=0;
    tmp_rf+=CH_Config.GWFRQ;
    tmp_rf=tmp_rf<<8;
    tmp_rf+=CH_Config.MaxNodeSize;
    tmp_rf=tmp_rf<<8;
    tmp_rf+=CH_Config.GWID;

    return tmp_rf;
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


