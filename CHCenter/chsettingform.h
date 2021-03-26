#ifndef CHSETTINGFORM_H
#define CHSETTINGFORM_H

#include <QWidget>
#include <QTimer>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>

namespace Ui
{
class CHSettingForm;
}

class CHSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit CHSettingForm(QWidget *parent = nullptr);
    ~CHSettingForm();


    void appendText(QString line);

public slots:
    void StreamATcmd();
    void settingConfig_init();

private slots:

    void sltMdbusParamLoaded();
    void on_LoadParamBTN_clicked();

    void on_BTN_clearTB_clicked();

    void on_CB_PTL_activated(int index);

    void on_CB_90_clicked();
    void on_CB_A0_clicked();
    void on_CB_B0_clicked();
    void on_CB_C0_clicked();
    void on_CB_D0_clicked();
    void on_CB_D1_clicked();
    void on_CB_F0_clicked();

    void on_CB_ODR_activated(const QString &arg1);

    void on_CB_Mode_activated(int index);

    void on_CB_Baud_activated(const QString &arg1);

    void on_BTN_ATCMD_clicked();

    void on_RSTBTN_clicked();

    void on_SB_GWID_valueChanged(int arg1);

    void on_SB_ID_valueChanged(int arg1);

    void on_CB_DongleParam_activated(int index);

signals:
    void sigSendATcmd(QString);
    void sigSetParam(char rw, uint32_t *param, int16_t address=-1);


protected:
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::CHSettingForm *ui;

    uint32_t m_modbus_param[112];


    struct CHConfig {

        QString Setptl="";
        QString Model="";

        uint32_t ID=0;
        int GWID=0, MaxNodeSize=0, GWFRQ=0;

        uint16_t Bitmap=0;
        uint16_t ODR=0;
        uint32_t Baud=0;
        uint32_t Mode=0;
    };
    CHConfig CH_Config;

    void identifyProduct();
    void writeUART_CFG();
    void writeCmd(uint8_t cmd);
    void delay(uint32_t ms);

};

#endif // CHSETTINGFORM_H
