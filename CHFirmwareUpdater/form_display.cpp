#include "form_display.h"
#include "ui_form_display.h"

#include "utilities/imu_parser.h"
#include <QDebug>
#include <QTimer>

uint32_t sample_cntr = 0;
uint32_t sample_frq = 0;

static uint8_t ui_ready = 0;

Form_display::Form_display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_display)
{
    ui->setupUi(this);

    this->parser = new imu_parser();

    /* creater timer for frq analysis */
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slt_tmr()));
    timer->start(1000);
    ui_ready = 1;

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
    int i;
    this->parser->parse(ba);
    QString text;

    sample_cntr++;
    text.append(QString("bitmap:%1\n").arg(parser->bitmap));
    text.append(QString("frq:%1\n").arg(sample_frq));

    if(parser->bitmap & BIT_RF_DONGLE)
    {
        text.append(QString("GWID:%1, CNT:%2\n").arg(parser->dev_info.gwid).arg(parser->dev_info.node_cnt));
    }
    else /* not RF doongle flag, only single IMU */
    {
        parser->dev_info.node_cnt = 1;
    }

    for(i=0; i<parser->dev_info.node_cnt; i++)
    {
        if(parser->bitmap & BIT_VALID_ID)
        {
            text.append(QString("ID:%1\n").arg(parser->dev[i].id));
        }

        if(parser->bitmap & BIT_VALID_ACC)
        {
            text.append(QString("ACC:%1,%2,%3\n").arg(parser->dev[i].acc[0]).arg(parser->dev[i].acc[1]).arg(parser->dev[i].acc[2]));
        }

        if(parser->bitmap & BIT_VALID_GYR)
        {
            text.append(QString("GYR:%1,%2,%3\n").arg(parser->dev[i].gyr[0]).arg(parser->dev[i].gyr[1]).arg(parser->dev[i].gyr[2]));
        }

        if(parser->bitmap & BIT_VALID_MAG)
        {
            text.append(QString("MAG:%1,%2,%3\n").arg(parser->dev[i].mag[0]).arg(parser->dev[i].mag[1]).arg(parser->dev[i].mag[2]));
        }

        if(parser->bitmap & BIT_VALID_EUL)
        {
            text.append(QString("EUL:%1,%2,%3\n").arg(parser->dev[i].eul[0]).arg(parser->dev[i].eul[1]).arg(parser->dev[i].eul[2]));
        }

        if(parser->bitmap & BIT_VALID_QUAT)
        {
            text.append(QString("QUAT:%1,%2,%3,%4\n").arg(parser->dev[i].quat[0]).arg(parser->dev[i].quat[1]).arg(parser->dev[i].quat[2]).arg(parser->dev[i].quat[3]));
        }
    }


    if(ui_ready)
    {
        ui->label->setText(text);
    }


}
