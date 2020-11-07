#ifndef COMFORM_H
#define COMFORM_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QListWidgetItem>

namespace Ui {
class ComForm;
}

class ComForm : public QWidget
{
    Q_OBJECT

public:
    explicit ComForm(QWidget *parent = nullptr);
    ~ComForm();


private slots:
    void on_BTNCancle_clicked();
    void on_BTNOK_clicked();
    void on_ListPort_itemClicked(QListWidgetItem *);
    void on_BTNPortRefresh_clicked();


signals:
    void sigPortChose(QString,int);
    void sigPortCancle();


private:
    Ui::ComForm *ui;
    QSerialPortInfo com_info;
};

#endif // COMFORM_H
