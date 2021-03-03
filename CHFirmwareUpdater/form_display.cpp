#include "form_display.h"
#include "ui_form_display.h"

#include <QDebug>

Form_display::Form_display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_display)
{
    ui->setupUi(this);
}

Form_display::~Form_display()
{
    delete ui;
}


void Form_display::slt_kptl_recv(QByteArray &ba)
{
    qDebug()<<"form_display slt_kptl_frame_recv:"<<ba.size();

}
