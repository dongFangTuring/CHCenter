#include "chsettingform.h"
#include "ui_chsettingform.h"

CHSettingForm::CHSettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHSettingForm)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Device Settings"));

    connect(this, SIGNAL(sigSendATcmd(QString)), this, SLOT(displayATcmd(QString)));
    autoRST=new QTimer(this);

    settingConfig_init();
    on_AdvancedCheckBox_clicked(false);


}

CHSettingForm::~CHSettingForm()
{
    delete autoRST;
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
    new_ch_config=old_ch_config;
    old_ch_config.Baud=0;
    old_ch_config.Setptl="";
    old_ch_config.FrameRate=-1;
    old_ch_config.GWID=0;
    old_ch_config.ID=1;
    old_ch_config.Mode=-1;

    if(old_ch_config.Setptl.indexOf("90")>=0)
        ui->IDCheckBox->setChecked(1);
    else
        ui->IDCheckBox->setChecked(0);
    if(old_ch_config.Setptl.indexOf("A0")>=0)
        ui->AccCheckBox->setChecked(1);
    else
        ui->AccCheckBox->setChecked(0);
    if(old_ch_config.Setptl.indexOf("B0")>=0)
        ui->GyroCheckBox->setChecked(1);
    else
        ui->GyroCheckBox->setChecked(0);
    if(old_ch_config.Setptl.indexOf("C0")>=0)
        ui->MagCheckBox->setChecked(1);
    else
        ui->MagCheckBox->setChecked(0);
    if(old_ch_config.Setptl.indexOf("D0")>=0)
        ui->EulerCheckBox->setChecked(1);
    else
        ui->EulerCheckBox->setChecked(0);
    if(old_ch_config.Setptl.indexOf("D1")>=0)
        ui->QuatCheckBox->setChecked(1);
    else
        ui->QuatCheckBox->setChecked(0);
    if(old_ch_config.Setptl.indexOf("91")>=0)
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


    if(old_ch_config.FrameRate==0)
        ui->FR0BTN->setChecked(1);
    else if(old_ch_config.FrameRate==1)
        ui->FR1BTN->setChecked(1);
    else if(old_ch_config.FrameRate==25)
        ui->FR25BTN->setChecked(1);
    else if(old_ch_config.FrameRate==50)
        ui->FR50BTN->setChecked(1);
    else if(old_ch_config.FrameRate==100)
        ui->FR100BTN->setChecked(1);
    else if(old_ch_config.FrameRate==400)
        ui->FR400BTN->setChecked(1);

    ui->IDInput->setValue(old_ch_config.ID);
    ui->GWIDInput->setValue(old_ch_config.GWID);

    ui->TerminalBox->clear();

    //stop output
    sigSendATcmd("AT+EOUT=0");
}
void CHSettingForm::settingConfig_leave()
{
    QString changes=old_ch_config.Compare(new_ch_config);
    if (changes!=""){
        QMessageBox msgBox;
        msgBox.setText(tr("The Config has been modified : \n%1").arg(changes));
        msgBox.setInformativeText(tr("Do you want to restart the module now?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        msgBox.setButtonText(QMessageBox::Yes, tr("Yes"));
        msgBox.setButtonText(QMessageBox::No, tr("Not now"));

        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Yes:
            emit sigSendATcmd("AT+RST");
            break;
        case QMessageBox::No:
            break;
        default:
            break;
        }

    }
    emit sigSendATcmd("AT+EOUT=1");
}

void CHSettingForm::setTerminalBoxText(QString str)
{
    ui->TerminalBox->append(str);

}

void CHSettingForm::StreamATcmd()
{
    emit sigSendATcmd("AT+EOUT=1");
}

void CHSettingForm::on_InfoBTN_clicked()
{
    emit sigSendATcmd("AT+INFO=L");
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

void CHSettingForm::on_StopStreamBTN_clicked()
{
    emit sigSendATcmd("AT+EOUT=0");
}



void CHSettingForm::on_BRSetBTN_clicked()
{

    if(ui->BR9600BTN->isChecked()){
        emit sigSendATcmd("AT+BAUD=9600");
        new_ch_config.Baud=9600;
    }
    else if(ui->BR115200BTN->isChecked()){
        emit sigSendATcmd("AT+BAUD=115200");
        new_ch_config.Baud=115200;
    }
    else if(ui->BR460800BTN->isChecked()){
        emit sigSendATcmd("AT+BAUD=460800");
        new_ch_config.Baud=460800;
    }
    else if(ui->BR921600BTN->isChecked()){
        emit sigSendATcmd("AT+BAUD=921600");
        new_ch_config.Baud=921600;
    }
}

void CHSettingForm::on_FRSetBTN_clicked()
{
    if(ui->FR1BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=1");
        new_ch_config.FrameRate=1;
    }
    else if(ui->FR0BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=0");
        new_ch_config.FrameRate=0;
    }
    else if(ui->FR25BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=25");
        new_ch_config.FrameRate=25;
    }
    else if(ui->FR50BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=50");
        new_ch_config.FrameRate=50;
    }
    else if(ui->FR100BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=100");
        new_ch_config.FrameRate=100;
    }
    else if(ui->FR400BTN->isChecked()){
        emit sigSendATcmd("AT+ODR=400");
        new_ch_config.FrameRate=400;
    }

}

void CHSettingForm::on_Mode6AxisBTN_clicked()
{
    emit sigSendATcmd("AT+MODE=0");
    new_ch_config.Mode=0;

}

void CHSettingForm::on_Mode9AxisBTN_clicked()
{
    emit sigSendATcmd("AT+MODE=1");
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
    emit sigSendATcmd("AT+ID="+ui->IDInput->text());
    new_ch_config.ID=ui->IDInput->text().toInt();
}

void CHSettingForm::on_GWIDSetBTN_clicked()
{
    emit sigSendATcmd("AT+GWID="+ui->GWIDInput->text());
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
