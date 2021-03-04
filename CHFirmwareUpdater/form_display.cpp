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


void Form_display::slt_kptl_payload_recv(QByteArray &ba)
{

    this->parser->parse(ba);
    QString text;

    sample_cntr++;
    text.append(QString("bitmap:%1\n").arg(parser->bitmap));
    text.append(QString("frq:%1\n").arg(sample_frq));

    if(parser->bitmap & BIT_RF_DONGLE)
    {
        text.append(QString("GWID:%1, CNT:%2\n").arg(parser->dev_info.gwid).arg(parser->dev_info.node_cnt));

    }

    if(parser->bitmap & BIT_VALID_ID)
    {
        text.append(QString("ID:%1\n").arg(parser->dev[0].id));
    }

    if(parser->bitmap & BIT_VALID_ACC)
    {
        text.append(QString("ACC:%1,%2,%3\n").arg(parser->dev[0].acc[0]).arg(parser->dev[0].acc[1]).arg(parser->dev[0].acc[2]));
    }

    if(parser->bitmap & BIT_VALID_GYR)
    {
        text.append(QString("GYR:%1,%2,%3\n").arg(parser->dev[0].gyr[0]).arg(parser->dev[0].gyr[1]).arg(parser->dev[0].gyr[2]));
    }

    if(parser->bitmap & BIT_VALID_MAG)
    {
        text.append(QString("MAG:%1,%2,%3\n").arg(parser->dev[0].mag[0]).arg(parser->dev[0].mag[1]).arg(parser->dev[0].mag[2]));
    }

    if(parser->bitmap & BIT_VALID_EUL)
    {
        text.append(QString("EUL:%1,%2,%3\n").arg(parser->dev[0].eul[0]).arg(parser->dev[0].eul[1]).arg(parser->dev[0].eul[2]));
    }

    if(parser->bitmap & BIT_VALID_QUAT)
    {
        text.append(QString("QUAT:%1,%2,%3,%4\n").arg(parser->dev[0].quat[0]).arg(parser->dev[0].quat[1]).arg(parser->dev[0].quat[2]).arg(parser->dev[0].quat[3]));
    }

      ui->label->setText(text);

}
