#ifndef CHSETTINGFORM_H
#define CHSETTINGFORM_H

#include <QWidget>
#include <QTimer>
#include <QMessageBox>
#include <QCloseEvent>
#include <QShowEvent>
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
    ~CHSettingForm() override;

    struct CHConfig {
        QString Setptl="";
        QString Model="";
        QString UUID="";
        uint32_t ID=0;
        int GWID=0, MaxNodeSize=0, GWFRQ=0;
        uint16_t Bitmap=0;
        uint16_t ODR=0;
        uint32_t Baud=0;
        uint32_t Mode=0;
    };


    void appendText(QString line);
    CHConfig getCHConfig(){
        on_LoadParamBTN_clicked();
        writeCmd(0x08); /* ENABLE OUTPUT */
        return CH_Config;
    }

public slots:


private slots:

    //control btn
    void sltMdbusParamLoaded(uint read_reg, uint reg_index);
    void on_LoadParamBTN_clicked();
    void on_RSTBTN_clicked();
    void on_BTN_clearTB_clicked();

    //old ptl
    void on_CB_PTL_activated(int index);

    //at cmd
    void on_BTN_ATCMD_clicked();

signals:
    void sigSendATcmd(QString);
    void sigSetParam(char rw, uint32_t *param, int16_t address=-1);


protected:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
private:
    Ui::CHSettingForm *ui;

    CHConfig CH_Config;
    uint32_t tmp_cmd;

    void settingConfig_init();
    void identifyProduct();
    void uiLoadConfig();

    uint32_t calcu_UART_CFG();
    uint32_t calcu_DongleParam();

    void writeCmd(uint8_t cmd);
    void delay(uint32_t ms);

};

#endif // CHSETTINGFORM_H
