#include "form_com_select.h"
#include "ui_form_com_select.h"

form_com_slect::form_com_slect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CHComForm)
{
    ui->setupUi(this);
    ui->BTNOK->setEnabled(0);
    //this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->ListPort->setSelectionMode(QAbstractItemView::SingleSelection);

    QStringList list = {"9600", tr("115200(Default)"), "460800", "921600"};
    ui->CBoxBaudrate->addItems(list);
    ui->CBoxBaudrate->setCurrentIndex(1);

    on_BTNPortRefresh_clicked();
}


form_com_slect::~form_com_slect()
{
    delete ui;
}

void form_com_slect::on_ListPort_itemClicked(QListWidgetItem *)
{
    ui->BTNOK->setEnabled(true);
}

/**
 * @brief CHComForm::on_BTNOK_clicked - emit port_name to baseform,
 * @param port_name - for example, port_name="COM1" in windows, "/dev/ttyUSB1" in Linux
 */
void form_com_slect::on_BTNOK_clicked()
{
    this->port_name = ui->ListPort->currentItem()->text().split(" : ").at(0);

    this->baud = 115200;
    switch (ui->CBoxBaudrate->currentIndex()) {
    case 0:
        this->baud = 9600;
        break;
    case 1:
        this->baud = 115200;
        break;
    case 2:
        this->baud = 460800;
        break;
    case 3:
        this->baud = 921600;
        break;
    default:
        this->baud = 115200;
        break;
    }

    this->accept();
}


void form_com_slect::on_BTNCancle_clicked()
{
    this->reject();
}

void form_com_slect::on_BTNPortRefresh_clicked()
{
    ui->BTNOK->setEnabled(false);
    ui->ListPort->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        com_info = info;
        ui->ListPort->addItem(com_info.portName() + " : " + com_info.description());
    }
}
