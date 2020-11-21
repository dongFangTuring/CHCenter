#ifndef CHSETTINGFORM_H
#define CHSETTINGFORM_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class CHSettingForm;
}

class CHSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit CHSettingForm(QWidget *parent = nullptr);
    ~CHSettingForm();

public slots:
    void setTerminalBoxText(QString str);
    void on_StreamBTN_clicked();
    void on_StopStreamBTN_clicked();
    void settingConfig_init();

private slots:
    void on_InfoBTN_clicked();
    void on_TrgBTN_clicked();
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

    void on_GWIMUSOLCheckBox_clicked();

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
};

#endif // CHSETTINGFORM_H
