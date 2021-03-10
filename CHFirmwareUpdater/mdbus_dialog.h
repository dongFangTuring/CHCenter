#ifndef TEST_DIALOG_H
#define TEST_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QStatusBar>

#include "mdbus/mdbus.h"


namespace Ui {
class main_Dialog;
}

class mdbus_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit mdbus_Dialog(QWidget *parent = nullptr);
    ~mdbus_Dialog();

    void set_interface(mdbus *bus);

private slots:
    void on_btn_read_clicked();

    void on_btn_write_clicked();


    void on_btn_6axis_clicked();

    void on_btn_rst_clicked();

    void on_btn_9xias_clicked();

    void on_btn_save_clicked();

    void on_btn_read_param_clicked();

    void on_btn_read_info_clicked();

private:
    Ui::main_Dialog *ui;
    mdbus *bus;
};

#endif // TEST_DIALOG_H
