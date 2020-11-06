#include "comform.h"
#include "ui_comform.h"

ComForm::ComForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComForm)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color:white;");


    ui->BTNOK->setEnabled(0);

    //user can click only one item
    ui->ListPort->setSelectionMode(QAbstractItemView::SingleSelection);

    //add every serial port in computer to listwidget(ListPort)
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        com_info = info;
        ui->ListPort->addItem(com_info.portName()+" : "+com_info.description());
    }
    QStringList buadrate_list={"9600","115200(Default)","460800","921600"};
    ui->CBoxBaudrate->addItems(buadrate_list);
    ui->CBoxBaudrate->setCurrentIndex(1);

}


ComForm::~ComForm()
{
    delete ui;
}

/**
 * @brief ComForm::on_ListPort_itemClicked -
 * ok icon can be clicked only after user chose one item in listwidget.
 * @param port_name
 */
void ComForm::on_ListPort_itemClicked(QListWidgetItem *)
{

    ui->BTNOK->setEnabled(1);
    QString port_name = ui->ListPort->currentItem()->text().split(" : ").at(0);
    ui->LabelSelected->setText("Selected : " + port_name);
}

/**
 * @brief ComForm::on_BTNOK_clicked - emit port_name to baseform,
 * @param port_name - for example, port_name="COM1" in windows, "/dev/ttyUSB1" in Linux
 */
void ComForm::on_BTNOK_clicked()
{

    QString port_name = ui->ListPort->currentItem()->text().split(" : ").at(0);

    int baudrate=115200;
    switch (ui->CBoxBaudrate->currentIndex()) {
    case 0:
        baudrate=9600;
        break;
    case 1:
        baudrate=115200;
        break;
    case 2:
        baudrate=460800;
        break;
    case 3:
        baudrate=921600;
        break;
    default:
        baudrate=115200;
        break;
    }


    emit sig_port_chose(port_name,baudrate);
}

/**
 * @brief ComForm::on_BTNCancle_clicked - emit close signal to baseform
 */
void ComForm::on_BTNCancle_clicked()
{
    emit sig_port_cancle();
}

void ComForm::on_BTNPortRefresh_clicked()
{
    ui->ListPort->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        com_info = info;
        ui->ListPort->addItem(com_info.portName()+" : "+com_info.description());
    }
}
