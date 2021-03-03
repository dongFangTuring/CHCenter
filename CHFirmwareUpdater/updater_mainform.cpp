#include "updater_mainform.h"
#include "ui_updater_mainform.h"
#include <QSerialPortInfo>
#include <QLCDNumber>
#include <QThread>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QtEndian>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
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
    this->kboot = new kboot_protocol();

    this->bus = new mdbus();
    this->mdbus_diag = new mdbus_Dialog(this);
    this->mdbus_diag->set_interface(this->bus);

    this->disp_diag = new Form_display();


    scan_port();

    connect(ui->actionModbus, &QAction::triggered, [=]()
    {
//        mdbus_diag->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
        mdbus_diag->setWindowModality(Qt::WindowModal);
        mdbus_diag->setWindowTitle("mdbus test");
        mdbus_diag->show();
    }
    );

    connect(ui->actionDisplay, &QAction::triggered, [=]()
    {
        disp_diag->setWindowModality(Qt::WindowModal);
        disp_diag->setWindowTitle("display test");
        disp_diag->show();
    }
    );


    /* connect serial interface to mainwindow*/
    connect(mserial, &QSerialPort::errorOccurred, this, &MainWindow::slt_serial_error);
    connect(mserial, &QSerialPort::readyRead, this, &MainWindow::slt_serial_read);

    /* connect kboot paser to serial */
    connect(this, &MainWindow::sig_serial_send, this->kboot, &kboot_protocol::slt_serial_read);
    connect(this->kboot, &kboot_protocol::sig_serial_send, this, &MainWindow::slt_serial_send);


    /* connect kboot download progress */
    connect(this->kboot, &kboot_protocol::sig_download_progress, this, &MainWindow::slt_update_progress_bar);

    /* connect modbus decoder to serial */
    connect(this, &MainWindow::sig_serial_send, this->bus, &mdbus::slt_serial_read);
    connect(this->bus, &mdbus::sig_serial_send, this, &MainWindow::slt_serial_send);

    /* connect kptl recv to form_display */
    connect(this->kboot, &kboot_protocol::sig_frame_recv, this->disp_diag, &Form_display::slt_kptl_recv);

}

void MainWindow::open()
{
    QMessageBox::information(this, tr("Information"), tr("Open"));
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

void MainWindow:: slt_serial_send(QByteArray &ba)
{
    this->mserial->write(ba);

//    this->mserial->clear(QSerialPort::Input);

      this->mserial->waitForBytesWritten();
    //this->mserial->waitForReadyRead(20);
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

            this->setWindowTitle(QString("HIPNUC Updater - %1,%2").arg(portname).arg(ui->comboBox_baud->currentText()));
            ui->textEdit->insertPlainText(QString("Open serial port OK\n"));

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

void MainWindow::slt_serial_read(void)
{
    QByteArray ba = mserial->readAll();
    emit sig_serial_send(ba);
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
    this->mserial->write(ba);
    this->mserial->waitForBytesWritten();
    this->kboot->delay(20);


    if(this->ba_image.size() == 0)
    {
        ui->textEdit->insertPlainText(QString("NO IMAGE\r\n"));
        download_ui_reset_action(true);
        return;
    }

    if(this->kboot->connect() == false)
    {
        ui->textEdit->insertPlainText(QString("CONNECT ERR\r\n"));
        download_ui_reset_action(true);
        return;
    }

    ui->textEdit->insertPlainText("Connect OK\n");
    ui->textEdit->insertPlainText(QString("PKT SIZE:%1\n").arg(this->kboot->max_packet_size()));
    ui->textEdit->insertPlainText(QString("FLASH SIZE:%1KB\n").arg(this->kboot->flash_size()));
    ui->textEdit->insertPlainText("SDID:" + this->kboot->sdid() + '\n');
    ui->textEdit->insertPlainText(QString("KBOOT:%1.%2.%3\n").arg(this->kboot->ver_major()).arg(this->kboot->ver_minor()).arg(this->kboot->ver_bugfix()));
    ui->textEdit->insertPlainText(QString("SECTOR:%1\n").arg(this->kboot->flash_sec_size()));

    if(!this->kboot->download(ba_image, hex2bin::start_addr()))
    {
        ui->textEdit->insertPlainText(QString("Download failed\n"));
    }
    else
    {
        ui->textEdit->insertPlainText(QString::fromUtf8("Update complete, image running...\n"));
    }

    download_ui_reset_action(true);

}

