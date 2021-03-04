#include "form_display.h"
#include "ui_form_display.h"

#include "utilities/imu_parser.h"
#include <QDebug>
#include <QTimer>

 uint32_t sample_cntr = 0;
 uint32_t sample_frq = 0;

Form_display::Form_display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_display)
{
    ui->setupUi(this);

    this->parser = new imu_parser();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slt_tmr()));
    timer->start(1000);

}


Form_display::~Form_display()
{
    delete ui;
}

void Form_display::slt_tmr(void)
{
    sample_frq = sample_cntr;
    sample_cntr = 0;
}


void Form_display::slt_kptl_recv(QByteArray &ba)
{
    this->parser->parse(ba);
    QString text;

    sample_cntr++;
    text.append(QString("bitmap:%1\n").arg(parser->bitmap));
    text.append(QString("frq:%1\n").arg(sample_frq));

    if(parser->bitmap & BIT_VALID_ID)
    {
        text.append(QString("ID:%1\n").arg(parser->dev.id));
    }

    if(parser->bitmap & BIT_VALID_ACC)
    {
        text.append(QString("ACC:%1,%2,%3\n").arg(parser->dev.acc[0]).arg(parser->dev.acc[1]).arg(parser->dev.acc[2]));
    }

    if(parser->bitmap & BIT_VALID_GYR)
    {
        text.append(QString("GYR:%1,%2,%3\n").arg(parser->dev.gyr[0]).arg(parser->dev.gyr[1]).arg(parser->dev.gyr[2]));
    }

    if(parser->bitmap & BIT_VALID_MAG)
    {
        text.append(QString("MAG:%1,%2,%3\n").arg(parser->dev.mag[0]).arg(parser->dev.mag[1]).arg(parser->dev.mag[2]));
    }

    if(parser->bitmap & BIT_VALID_EUL)
    {
        text.append(QString("EUL:%1,%2,%3\n").arg(parser->dev.eul[0]).arg(parser->dev.eul[1]).arg(parser->dev.eul[2]));
    }

    if(parser->bitmap & BIT_VALID_QUAT)
    {
        text.append(QString("QUAT:%1,%2,%3,%4\n").arg(parser->dev.quat[0]).arg(parser->dev.quat[1]).arg(parser->dev.quat[2]).arg(parser->dev.quat[3]));
    }

      ui->label->setText(text);

}
