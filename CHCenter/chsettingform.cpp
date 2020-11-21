#include "chsettingform.h"
#include "ui_chsettingform.h"
#include <QDebug>
CHSettingForm::CHSettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHSettingForm)
{
    ui->setupUi(this);

    connect(this, SIGNAL(sigSendATcmd(QString)), this, SLOT(displayATcmd(QString)));
    autoRST=new QTimer(this);
    settingConfig_init();
}

CHSettingForm::~CHSettingForm()
{
    delete ui;
}

void CHSettingForm::settingConfig_init()
{
    ui->BR115200BTN->setChecked(1);
    ui->FR100BTN->setChecked(1);
}

void CHSettingForm::setTerminalBoxText(QString str)
{
    ui->TerminalBox->append(str);
}

void CHSettingForm::on_InfoBTN_clicked()
{
    emit sigSendATcmd("AT+INFO=L");
}


void CHSettingForm::on_TrgBTN_clicked()
{
    emit sigSendATcmd("AT+TRG");
}

void CHSettingForm::on_RestartBTN_clicked()
{
    emit sigSendATcmd("AT+RST");
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

void CHSettingForm::on_StreamBTN_clicked()
{
    emit sigSendATcmd("AT+EOUT=1");
}

void CHSettingForm::on_StopStreamBTN_clicked()
{
    emit sigSendATcmd("AT+EOUT=0");
}



void CHSettingForm::on_BRSetBTN_clicked()
{

    if(ui->BR9600BTN->isChecked()){
        emit sigSendATcmd("AT+BAUD=9600");
        QTimer::singleShot(1000, this, SLOT(on_RestartBTN_clicked()));
    }
    else if(ui->BR115200BTN->isChecked()){
        emit sigSendATcmd("AT+BAUD=115200");
        QTimer::singleShot(1000, this, SLOT(on_RestartBTN_clicked()));
    }
    else if(ui->BR460800BTN->isChecked()){
        emit sigSendATcmd("AT+BAUD=460800");
        QTimer::singleShot(1000, this, SLOT(on_RestartBTN_clicked()));
    }
    else if(ui->BR921600BTN->isChecked()){
        emit sigSendATcmd("AT+BAUD=921600");
        QTimer::singleShot(1000, this, SLOT(on_RestartBTN_clicked()));
    }

}

void CHSettingForm::on_FRSetBTN_clicked()
{
    if(ui->FR1BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=1");
        QTimer::singleShot(1000, this, SLOT(on_RestartBTN_clicked()));
    }
    else if(ui->FR25BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=25");
        QTimer::singleShot(1000, this, SLOT(on_RestartBTN_clicked()));
    }
    else if(ui->FR50BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=50");
        QTimer::singleShot(1000, this, SLOT(on_RestartBTN_clicked()));
    }
    else if(ui->FR100BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=100");
        QTimer::singleShot(1000, this, SLOT(on_RestartBTN_clicked()));
    }

}

void CHSettingForm::on_Mode6AxisBTN_clicked()
{
    emit sigSendATcmd("AT+MODE=0");
    QTimer::singleShot(1000, this, SLOT(on_RestartBTN_clicked()));
}

void CHSettingForm::on_Mode9AxisBTN_clicked()
{
    emit sigSendATcmd("AT+MODE=1");
    QTimer::singleShot(1000, this, SLOT(on_RestartBTN_clicked()));
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

    if(ui->IMUSOLCheckBox->isChecked()){
        setptl="AT+SETPTL=91";
        ui->IDCheckBox->setChecked(0);
        ui->AccCheckBox->setChecked(0);
        ui->GyroCheckBox->setChecked(0);
        ui->MagCheckBox->setChecked(0);
        ui->EulerCheckBox->setChecked(0);
        ui->QuatCheckBox->setChecked(0);
    }
    if(ui->GWIMUSOLCheckBox->isChecked()){
        setptl="AT+SETPTL=62";
        ui->IDCheckBox->setChecked(0);
        ui->AccCheckBox->setChecked(0);
        ui->GyroCheckBox->setChecked(0);
        ui->MagCheckBox->setChecked(0);
        ui->EulerCheckBox->setChecked(0);
        ui->QuatCheckBox->setChecked(0);
    }
    emit sigSendATcmd(setptl);
    QTimer::singleShot(1000, this, SLOT(on_RestartBTN_clicked()));
}

void CHSettingForm::on_IMUSOLCheckBox_clicked()
{
    if(ui->IMUSOLCheckBox->isChecked()){
        ui->IDCheckBox->setChecked(0);
        ui->AccCheckBox->setChecked(0);
        ui->GyroCheckBox->setChecked(0);
        ui->MagCheckBox->setChecked(0);
        ui->EulerCheckBox->setChecked(0);
        ui->QuatCheckBox->setChecked(0);
        ui->GWIMUSOLCheckBox->setChecked(0);
    }
}

void CHSettingForm::on_GWIMUSOLCheckBox_clicked()
{
    if(ui->GWIMUSOLCheckBox->isChecked()){
        ui->IDCheckBox->setChecked(0);
        ui->AccCheckBox->setChecked(0);
        ui->GyroCheckBox->setChecked(0);
        ui->MagCheckBox->setChecked(0);
        ui->EulerCheckBox->setChecked(0);
        ui->QuatCheckBox->setChecked(0);
        ui->IMUSOLCheckBox->setChecked(0);
    }
}

void CHSettingForm::on_IDCheckBox_clicked()
{
    ui->IMUSOLCheckBox->setChecked(0);
    ui->GWIMUSOLCheckBox->setChecked(0);
}

void CHSettingForm::on_AccCheckBox_clicked()
{
    ui->IMUSOLCheckBox->setChecked(0);
    ui->GWIMUSOLCheckBox->setChecked(0);
}

void CHSettingForm::on_GyroCheckBox_clicked()
{
    ui->IMUSOLCheckBox->setChecked(0);
    ui->GWIMUSOLCheckBox->setChecked(0);
}

void CHSettingForm::on_MagCheckBox_clicked()
{
    ui->IMUSOLCheckBox->setChecked(0);
    ui->GWIMUSOLCheckBox->setChecked(0);
}

void CHSettingForm::on_EulerCheckBox_clicked()
{
    ui->IMUSOLCheckBox->setChecked(0);
    ui->GWIMUSOLCheckBox->setChecked(0);
}

void CHSettingForm::on_QuatCheckBox_clicked()
{
    ui->IMUSOLCheckBox->setChecked(0);
    ui->GWIMUSOLCheckBox->setChecked(0);
}

void CHSettingForm::on_IDSetBTN_clicked()
{
    emit sigSendATcmd("AT+ID="+ui->IDInput->text());
    QTimer::singleShot(1000, this, SLOT(on_RestartBTN_clicked()));
}

void CHSettingForm::on_GWIDSetBTN_clicked()
{
    emit sigSendATcmd("AT+GWID="+ui->GWIDInput->text());
    QTimer::singleShot(1000, this, SLOT(on_RestartBTN_clicked()));
}
