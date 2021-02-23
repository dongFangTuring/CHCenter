#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QLCDNumber>
#include <QThread>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QtEndian>

#include "utilities/test2.h"

#include "mdbus_dialog.h"

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

    //test2 tes22;
    //test2 * mtest = new test2(3);



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

void MainWindow:: slt_update_progress_bar(int precent)
{
    ui->progressBar->setValue(precent);
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

            this->setWindowTitle(QString("HIPNUC Updater - %1,%2").arg(portname).arg(ui->comboBox_baud->currentText()));
            ui->textEdit->insertPlainText(QString("Open serial port OK\n"));
            kboot = new kboot_protocol(mserial);
            connect(kboot, &kboot_protocol::sig_download_progress, this, &MainWindow::slt_update_progress_bar);
            connect(mserial, &QSerialPort::errorOccurred, this, &MainWindow::slt_serial_error);
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

                /* print SP and PC */
                int sp = qFromLittleEndian<int>(ba_image.mid(0, 4));
                ui->textEdit->insertPlainText(QString("SP:0x%1\n").arg(sp, 8, 16, QLatin1Char('0')).toUpper());
                int pc = qFromLittleEndian<int>(ba_image.mid(4, 8));
                ui->textEdit->insertPlainText(QString("PC:0x%1\n").arg(pc, 8, 16, QLatin1Char('0')).toUpper());

                /* image start addr */
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
    kboot->serial_send_then_recv(ba, ba, 9999, 50);

    uint8_t ver_major;
    uint8_t ver_minor;
    uint8_t ver_bugfix;

    if(kboot->ping(ver_bugfix, ver_minor, ver_major) == false)
    {
        ui->textEdit->insertPlainText(QString("CONNECT ERR\r\n"));
        download_ui_reset_action(true);
        return;
    }


    /* version */
    ui->textEdit->insertPlainText(QString("KBOOT:%1.%2.%3\n").arg(ver_major).arg(ver_minor).arg(ver_bugfix));


    /* version */
    ba = kboot->cmd_get_property(0x01);
    ui->textEdit->insertPlainText(QString("%1.%2.%3\n").arg(uint8_t(ba.at(2))).arg(uint8_t(ba[1])).arg(uint8_t(ba[0])));


    /* get max packet size */
    ba = kboot->cmd_get_property(0x0B);
    if(ba.size() == 4)
    {
        this->max_packet_size = qFromLittleEndian<int>(ba.data());
        ui->textEdit->insertPlainText(QString("PKT SIZE:%1\n").arg(this->max_packet_size));
    }

    /* flash size */
    ba = kboot->cmd_get_property(0x04);
    if(ba.size() == 4)
    {
        ui->textEdit->insertPlainText(QString("FLASH SIZE:%1KB\n").arg(qFromLittleEndian<int>(ba.data()) / 1024));
    }

    //SDID
    ba = kboot->cmd_get_property(0x10);
    if(ba.size() == 4)
    {
        ui->textEdit->insertPlainText("SDID:" + ba.toHex() + '\n');
    }

    //Flash Sector Size
    ba = kboot->cmd_get_property(0x05);
    if(ba.size() == 4)
    {
        ui->textEdit->insertPlainText(QString("SECTOR:%1\n").arg(qFromLittleEndian<int>(ba.data())));
    }

    ui->textEdit->insertPlainText("Connect OK\n");

    if(!kboot->download(ba_image, hex2bin::start_addr(), this->max_packet_size))
    {
        ui->textEdit->insertPlainText(QString("Download failed\n"));
    }
    else
    {
        ui->textEdit->insertPlainText(QString::fromUtf8("Update complete, image running...\n"));
    }

    download_ui_reset_action(true);

}

void MainWindow::on_btn_test_clicked()
{
    mdbus_Dialog *diag = new mdbus_Dialog(this);

    diag->setWindowFlag(Qt::Window,true);           //将窗体设置为窗口属性
    diag->setWindowTitle("mdbus test");                  //设置新窗体的标题
    diag->show();

}
