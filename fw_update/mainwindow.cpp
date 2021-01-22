#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QLCDNumber>
#include <QThread>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QtEndian>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("HiPNUC Updater"));
    ui->comboBox_baud->setCurrentIndex(1); /* 115200 */
    ui->groupBox_2->setEnabled(false);
    mserial = new serial();
    scan_port();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::scan_port()
{
    QStringList list;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {

        list.append(info.portName()+":"+info.description());
    }
    list.sort();


    ui->comboBox_com->clear();
    QString str;
    foreach(QString str, list)
    {
        ui->comboBox_com->addItem(str);
    }
}


void MainWindow::on_btn_serial_open_clicked()
{
    if(ui->btn_serial_open->text() == tr("Open"))
    {
        /* open serial port */
        QString portname=ui->comboBox_com->currentText().split(":").first();

        if(mserial->open(portname, ui->comboBox_baud->currentText().toInt()))
        {
            ui->comboBox_com->setEnabled(false);
            ui->comboBox_baud->setEnabled(false);
            ui->btn_serial_open->setText(tr("Close"));
            ui->groupBox_2->setEnabled(true);

            // 當下位機中有數據發送過來時就會響應這個槽函數
            //connect(mserial, &serial::sig_rx_rdy, this, &MainWindow::slt_read_serial);
            connect(mserial, &QSerialPort::errorOccurred, this, &MainWindow::slt_serial_error);
            this->setWindowTitle(QString("HIPNUC Updater - %1,%2").arg(portname).arg(ui->comboBox_baud->currentText()));
            ui->textEdit->insertPlainText(QString("Open serial port OK\n"));
            kboot = new kboot_protocol(mserial);
        }
        else
        {
            ui->textEdit->insertPlainText(QString("Open serial port ERR\n"));
        }
    }
    else
    {
        serial_close_ui_action();
    }

}

void MainWindow::serial_close_ui_action()
{
    mserial->close();
    ui->comboBox_com->setEnabled(true);
    ui->comboBox_baud->setEnabled(true);
    ui->groupBox_2->setEnabled(false);
    ui->textEdit->insertPlainText(QString("Close serial port OK\n"));
    ui->btn_serial_open->setText(tr("Open"));
    this->setWindowTitle(tr("HIPNUC Updater"));
}

void MainWindow::slt_serial_error(QSerialPort::SerialPortError error) // 讀取從自定義序列埠類獲得的數據
{
    if(error == QSerialPort::ResourceError)
    {
        qDebug("SerialPortError:%d\r\n", error);
        QMessageBox::critical(this, "ERROR", "SERIAL ERROR");
        //qApp->quit();

        download_ui_reset_action(false);
        serial_close_ui_action();
    }
}


void MainWindow::on_btn_open_file_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Open File"), ".",  tr("Text Files(*.hex)"));

    QString filename=path.split("/").last();
    ui->label_file->setText(filename);

    if(!path.isEmpty())
    {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly| QIODevice::Text))
        {
            QTextStream text_stream(&file);

            ba_image = hex2bin::convert(text_stream);

            if(ba_image.size() > 0)
            {
                ui->textEdit->insertPlainText(QString("PATH:%1\n").arg(path));
                ui->textEdit->insertPlainText(QString("SIZE:%1\n").arg(ba_image.size()));

                /* 打印PC 和 SP */
                int sp = qFromLittleEndian<int>(ba_image.mid(0, 4));
                ui->textEdit->insertPlainText(QString("SP:0x%1\n").arg(sp, 8, 16, QLatin1Char('0')).toUpper());
                int pc = qFromLittleEndian<int>(ba_image.mid(4, 8));
                ui->textEdit->insertPlainText(QString("PC:0x%1\n").arg(pc, 8, 16, QLatin1Char('0')).toUpper());

                /* 起始地址 */
                this->start_addr = hex2bin::start_addr();
                ui->textEdit->insertPlainText(QString("ADDR:0x%1\n").arg(this->start_addr, 8, 16, QLatin1Char('0')).toUpper());

            }
            file.close();
        }
    }
}

void MainWindow::on_btn_reflash_com_clicked()
{
    scan_port();
}

void MainWindow::download_ui_reset_action(bool enabled)
{
    ui->groupBox_2->setEnabled(enabled);
    ui->progressBar->setValue(0);
}

void MainWindow::on_btn_program_clicked()
{
    QByteArray ba;

    ui->textEdit->clear();
    ui->groupBox_2->setEnabled(false);
    ui->progressBar->setValue(0);

    ba = QByteArray::fromRawData("AT+RST\r\n", 8);
    kboot->serial_send_then_recv(ba, 20);

    uint8_t slv_ver_major;
    uint8_t slv_ver_minor;
    uint8_t slv_ver_bugfix;

    if(!kboot->ping(slv_ver_bugfix, slv_ver_minor, slv_ver_major))
    {
        ui->textEdit->insertPlainText(QString("CONNECT ERR\r\n"));
        download_ui_reset_action(true);
        return;
    }

    ui->textEdit->insertPlainText(QString("KBOOT:%1.%2.%3\n").arg(slv_ver_major).arg(slv_ver_minor).arg(slv_ver_bugfix));

    /* version */
    ba = kboot->get_property(0x01);

    /* get max packet size */
    ba = kboot->get_property(0x0B);
    if(ba.size() == 4)
    {
        this->max_packet_size = qFromLittleEndian<int>(ba.data());
        ui->textEdit->insertPlainText(QString("PKT SIZE:%1\n").arg(this->max_packet_size));
    }

    /* flash size */
    ba = kboot->get_property(0x04);
    if(ba.size() == 4)
    {
        ui->textEdit->insertPlainText(QString("FLASH SIZE:%1KB\n").arg(qFromLittleEndian<int>(ba.data()) / 1024));
    }

    //SDID
    ba = kboot->get_property(0x10);
    if(ba.size() == 4)
    {
        ui->textEdit->insertPlainText("SDID:" + ba.toHex() + '\n');
    }

    //Flash Sector Size
    ba = kboot->get_property(0x05);
    if(ba.size() == 4)
    {
        ui->textEdit->insertPlainText(QString("SECTOR:%1\n").arg(qFromLittleEndian<int>(ba.data())));
    }

    ui->textEdit->insertPlainText("Connect OK\n");

    /* image available */
    if(ba_image.size() > 0)
    {
        if(!kboot->flash_erase_region(hex2bin::start_addr(), ba_image.size()))
        {
            ui->textEdit->insertPlainText(QString("Erase failed\n"));
            download_ui_reset_action(true);
            return;
        }

        int image_size = ba_image.size();
        int i = 0;

        ui->textEdit->insertPlainText(QString("Programming...\n"));
        if(!kboot->flash_write_memory(hex2bin::start_addr(), ba_image.size()))
        {
            ui->textEdit->insertPlainText(QString("Write memory failed\n"));
            download_ui_reset_action(true);
            return;
        }

        while(i < image_size)
        {

            QByteArray slice = ba_image.mid(i, (image_size - i)>this->max_packet_size?(this->max_packet_size):(image_size-1));

            if(!kboot->send_data_packet(slice, (slice.size() == this->max_packet_size)?(false):(true)))
            {
                ui->textEdit->insertPlainText(QString("Send packet ERR\n"));
            }
            i += slice.size();
            ui->progressBar->setValue(i*100 / image_size);
        }

        kboot->reset();
        ui->textEdit->insertPlainText(QString::fromUtf8("Update complete, image running...\n"));
        download_ui_reset_action(true);
    }
    else
    {
        download_ui_reset_action(true);
        ui->textEdit->insertPlainText("No file\n");
        return;
    }

}
