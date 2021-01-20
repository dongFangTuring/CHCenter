#ifndef CHComForm_H
#define CHComForm_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QListWidgetItem>



namespace Ui {
class CHComForm;
}

class CHComForm : public QWidget
{
    Q_OBJECT

public:
    explicit CHComForm(QWidget *parent = nullptr);
    ~CHComForm();

public slots:
    void on_BTNPortRefresh_clicked();

private slots:
    void on_BTNCancle_clicked();
    void on_BTNOK_clicked();
    void on_ListPort_itemClicked(QListWidgetItem *);

signals:

    //send to baseform
    void sigPortChose(QString,int);
    void sigPortCancle();


private:
    Ui::CHComForm *ui;
    QSerialPortInfo com_info;
};

#endif // CHComForm_H
