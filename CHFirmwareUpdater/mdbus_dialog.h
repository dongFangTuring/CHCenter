#ifndef TEST_DIALOG_H
#define TEST_DIALOG_H

#include <QDialog>

namespace Ui {
class main_Dialog;
}

class mdbus_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit mdbus_Dialog(QWidget *parent = nullptr);
    ~mdbus_Dialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::main_Dialog *ui;
};

#endif // TEST_DIALOG_H
