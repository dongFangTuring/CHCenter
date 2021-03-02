#include "mdbus_dialog.h"
#include "ui_mdbus_dialog.h"
#include <QtCore>


mdbus_Dialog::mdbus_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::main_Dialog)
{
    ui->setupUi(this);
    ui->tableWidget_read->setColumnCount(3);
    ui->tableWidget_read->setRowCount(ui->spinBox_len->value());

    /* add headers */
    QStringList headers;
    headers<<"reg_idx"<<"int"<<"hex"<<"float";
    ui->tableWidget_read->setHorizontalHeaderLabels(headers);

}

mdbus_Dialog::~mdbus_Dialog()
{
    delete ui;
}

void mdbus_Dialog::set_interface(mdbus *bus)
{
    this->bus = bus;
}


void mdbus_Dialog::on_btn_read_clicked()
{
    bool ret;

    uint8_t slv_addr = ui->spinBox_slv_addr->value();
    uint16_t addr = ui->spinBox_reg_addr->value();
    uint16_t len = ui->spinBox_len->value();

    ui->tableWidget_read->setRowCount(len);

    ui->plainTextEdit->insertPlainText(QString("READ addr:%1, len:%2 ").arg(addr).arg(len));

    uint32_t buf[64];
    ret = this->bus->read_data(slv_addr, addr, buf, len);

    if(ret)
    {
        ui->plainTextEdit->insertPlainText("OK\n");
        for(int i=0; i<len; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(addr+i));
            item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
            ui->tableWidget_read->setItem(i, 0, item);

            item = new QTableWidgetItem(QString::number(buf[i])); /* parse as int */
            ui->tableWidget_read->setItem(i, 1, item);

            item = new QTableWidgetItem("0x" + QString::number(buf[i], 16));   /* parse as hex */
            ui->tableWidget_read->setItem(i, 2, item);
        }
    }
    else
    {
        ui->plainTextEdit->insertPlainText("ERR\n");
    }

}


void mdbus_Dialog::on_btn_write_clicked()
{
    bool ret;

    uint8_t slv_addr = ui->spinBox_slv_addr->value();
    uint16_t addr = ui->spinBox_reg_addr->value();

    QByteArray ba;

    /* collet data */
    uint32_t buf[64];

    for(int i=0; i<ui->tableWidget_read->rowCount(); i++)
    {
        QString text = ui->tableWidget_read->item(i, 1)->text();
        buf[i] = text.toInt();
    }

    ui->plainTextEdit->insertPlainText(QString("WRITE addr:%1, len:%2 ").arg(addr).arg(ui->tableWidget_read->rowCount()));

    ret = this->bus->write_data(slv_addr, addr, buf, ui->tableWidget_read->rowCount());

    if(ret)
        ui->plainTextEdit->insertPlainText("OK\n");
    else
        ui->plainTextEdit->insertPlainText("ERR\n");
}




void mdbus_Dialog::on_btn_6axis_clicked()
{
    /* SLV_ADDR =1,  REG_ADDR=16, VAL = 0x01 */
    this->bus->write_reg(1, 16, 0x01);
}

void mdbus_Dialog::on_btn_rst_clicked()
{
    this->bus->write_reg(1, 17, 0x05);
}

void mdbus_Dialog::on_btn_9xias_clicked()
{
    this->bus->write_reg(1, 16, 0x03);
}

void mdbus_Dialog::on_btn_save_clicked()
{
    this->bus->write_reg(1, 17, 0x06);
}

void mdbus_Dialog::on_btn_read_param_clicked()
{
    uint32_t buf[12];
    QString text;

    /* ACC */
    this->bus->read_data(1, 64+3, buf, 12);

    text = "ACC:\n" +
            QString("%1 %2 %3\n").arg(QString::number(*(float*)&buf[0], 'f', 4)).arg(QString::number(*(float*)&buf[1], 'f', 4)).arg(QString::number(*(float*)&buf[2], 'f', 4)) +
            QString("%1 %2 %3\n").arg(QString::number(*(float*)&buf[3], 'f', 4)).arg(QString::number(*(float*)&buf[4], 'f', 4)).arg(QString::number(*(float*)&buf[5], 'f', 4)) +
            QString("%1 %2 %3\n").arg(QString::number(*(float*)&buf[6], 'f', 4)).arg(QString::number(*(float*)&buf[7], 'f', 4)).arg(QString::number(*(float*)&buf[8], 'f', 4)) +
            QString("%1 %2 %3\n").arg(QString::number(*(float*)&buf[9], 'f', 4)).arg(QString::number(*(float*)&buf[10], 'f', 4)).arg(QString::number(*(float*)&buf[11], 'f', 4));

    ui->label_param->setText(text);


}
