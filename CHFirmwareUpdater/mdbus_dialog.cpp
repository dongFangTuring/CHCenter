#include "mdbus_dialog.h"
#include "ui_mdbus_dialog.h"

mdbus_Dialog::mdbus_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::main_Dialog)
{
    ui->setupUi(this);
}

mdbus_Dialog::~mdbus_Dialog()
{
    delete ui;
}

void mdbus_Dialog::on_pushButton_clicked()
{
    this->hide();
}
