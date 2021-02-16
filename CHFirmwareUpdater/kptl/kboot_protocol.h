#ifndef KBOOT_PROTOCOL_H
#define KBOOT_PROTOCOL_H

#include <QtCore>
#include <QSerialPort>
#include "kptl/kptl.h"


class kboot_protocol:public QObject
{
    Q_OBJECT

public:
    kboot_protocol(QSerialPort *mserial);
    ~kboot_protocol();
    bool serial_send_then_recv(QByteArray &tx, QByteArray &rx, int expected_len, int timeout = 400);
    bool ping(uint8_t &bugfix, uint8_t &minor, uint8_t &major);
    QByteArray cmd_get_property(uint8_t property_code);
    bool cmd_flash_erase_region(uint32_t addr, uint32_t len);
    bool cmd_flash_write_memory(uint32_t addr, uint32_t len);
    bool cmd_send_data_packet(QByteArray &buf, bool is_last);
    bool cmd_reset();
    bool download(QByteArray image, int start_addr, int max_packet_size, int retry = 3);

private:
    QSerialPort *mserial;
    pkt_dec_t dec;
    void delay(uint32_t ms);
    QByteArray cmd_packet(uint8_t tag, uint8_t param_cnt, uint32_t *param, int expected_len);

signals:
    void sig_download_progress(int precent);


};

#endif // KBOOT_PROTOCOL_H
