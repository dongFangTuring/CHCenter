#ifndef CHComForm_H
#define CHComForm_H

#include <QSerialPort>
#include <QDialog>
#include <QSerialPortInfo>
#include <QListWidgetItem>



namespace Ui
{
class CHComForm;
}

class form_com_slect : public QDialog
{
    Q_OBJECT

public:
    explicit form_com_slect(QWidget *parent = nullptr);
    ~form_com_slect() override;
    QString port_name;
    int baud;
private slots:
    void on_BTNCancle_clicked();
    void on_BTNOK_clicked();
    void on_ListPort_itemClicked(QListWidgetItem *);
    void on_BTNPortRefresh_clicked();


private:
    Ui::CHComForm *ui;
    QSerialPortInfo com_info;
};

#endif // CHComForm_H
