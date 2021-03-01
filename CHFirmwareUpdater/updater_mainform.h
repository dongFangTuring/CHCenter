#ifndef UPDATER_MAINFORM_H
#define UPDATER_MAINFORM_H

#include <QMainWindow>


#include "kptl/kptl.h"
#include "kptl/kboot_protocol.h"
#include "utilities/serial.h"
#include "utilities/hex2bin.h"

#include "mdbus_dialog.h"
#include "mdbus/mdbus.h"

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
    void on_btn_reflash_com_clicked();
    void on_btn_program_clicked();

    void slt_serial_error(QSerialPort::SerialPortError);
    void slt_serial_read(void);
    void slt_update_progress_bar(int precent);
    void slt_serial_send(QByteArray &ba);

signals:
    void sig_serial_send(QByteArray &ba);

private:
    mdbus_Dialog *mdbus_diag;

    QByteArray ba_image;
    serial *mserial;
    kboot_protocol *kboot;
    mdbus *bus;

    int start_addr;
    void scan_port(void);
    void open();
    void download_ui_reset_action(bool enabled);
    void serial_close_ui_action();
    Ui::MainWindow *ui;
};
#endif // UPDATER_MAINFORM_H
