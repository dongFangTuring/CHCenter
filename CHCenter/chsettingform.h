#ifndef CHSETTINGFORM_H
#define CHSETTINGFORM_H

#include <QWidget>
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class CHSettingForm;
}

class CHSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit CHSettingForm(QWidget *parent = nullptr);
    ~CHSettingForm();

    struct CHConfig{
        QString Setptl="";
        int FrameRate=-1;
        int Baud=-1;
        int ID=0;
        int GWID=0;
        int Mode=-1;

        QString Compare(CHConfig new_ch_config){
            QString changes="";
            if(this->FrameRate!=new_ch_config.FrameRate)
                changes+=tr("Frame Rate = %1\n").arg(QString::number(new_ch_config.FrameRate));
            if(this->Baud!=new_ch_config.Baud)
                changes+=tr("Baudrate = %1\n").arg(QString::number(new_ch_config.Baud));
            if(this->Setptl!=new_ch_config.Setptl)
                changes+=tr("Data Content = %1\n").arg(new_ch_config.Setptl);
            if(this->ID!=new_ch_config.ID)
                changes+=tr("Node ID = %1\n").arg(QString::number(new_ch_config.ID));
            if(this->GWID!=new_ch_config.GWID)
                changes+=tr("GWID = %1\n").arg(QString::number(new_ch_config.GWID));
            if(this->Mode!=new_ch_config.Mode)
                changes+=tr("Mode = %1\n").arg(QString::number(new_ch_config.Mode));
            return changes;
        }
    };
    CHConfig old_ch_config;

public slots:
    void setTerminalBoxText(QString str);
    void StreamATcmd();
    void on_StopStreamBTN_clicked();
    void settingConfig_init();
    void settingConfig_leave();

private slots:
    void on_InfoBTN_clicked();
    void on_RestartBTN_clicked();
    void displayATcmd(QString);
    void on_sendATcmdBTN_clicked();
    void on_clearBTN_clicked();
    void on_BRSetBTN_clicked();
    void on_FRSetBTN_clicked();
    void on_Mode6AxisBTN_clicked();
    void on_Mode9AxisBTN_clicked();
    void on_ProtocolSetBTN_clicked();
    void on_IMUSOLCheckBox_clicked();
    void on_IDCheckBox_clicked();
    void on_AccCheckBox_clicked();
    void on_GyroCheckBox_clicked();
    void on_MagCheckBox_clicked();
    void on_EulerCheckBox_clicked();
    void on_QuatCheckBox_clicked();
    void on_IDSetBTN_clicked();
    void on_GWIDSetBTN_clicked();

signals:
    void sigSendATcmd(QString);

private:
    Ui::CHSettingForm *ui;
    QTimer *autoRST;

    CHConfig new_ch_config;

};

#endif // CHSETTINGFORM_H
