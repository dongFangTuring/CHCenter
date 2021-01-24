#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "utilities/serial.h"
#include "kptl/kptl.h"
#include "kptl/kboot_protocol.h"
#include "utilities/hex2bin.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_btn_serial_open_clicked();
    void on_btn_open_file_clicked();

    void slt_serial_error(QSerialPort::SerialPortError);
    void on_btn_reflash_com_clicked();
    void on_btn_program_clicked();

signals:

private:
    QByteArray ba_image;
    serial *mserial;
    kboot_protocol *kboot;
    int start_addr;
    int max_packet_size;
    void scan_port(void);
    void download_ui_reset_action(bool enabled);
    void serial_close_ui_action();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
