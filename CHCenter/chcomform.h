#ifndef CHComForm_H
#define CHComForm_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QListWidgetItem>



namespace Ui
{
class CHComForm;
}

class CHComForm : public QWidget
{
    Q_OBJECT

public:
    explicit CHComForm(QWidget *parent = nullptr);
    ~CHComForm() override;

private slots:
    void on_BTNCancle_clicked();
    void on_BTNOK_clicked();
    void on_ListPort_itemClicked(QListWidgetItem *);
    void on_BTNPortRefresh_clicked();
signals:

    //send to baseform
    void sig_port_ok(QString,int);
    void sig_port_cancel();


private:
    Ui::CHComForm *ui;
    QSerialPortInfo com_info;
};

#endif // CHComForm_H
