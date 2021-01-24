#ifndef KBOOT_PROTOCOL_H
#define KBOOT_PROTOCOL_H

#include <QtCore>
#include <QSerialPort>
#include "kptl/kptl.h"

class kboot_protocol
{
public:
    kboot_protocol(QSerialPort *mserial);
    ~kboot_protocol();
    QByteArray serial_send_then_recv(QByteArray &tx, uint32_t ms = 20);
    bool ping(uint8_t &bugfix, uint8_t &minor, uint8_t &major);
    QByteArray get_property(uint8_t property_code);
    bool flash_erase_region(uint32_t addr, uint32_t len);
    bool flash_write_memory(uint32_t addr, uint32_t len);
    bool send_data_packet(QByteArray &buf, bool is_last);
    bool reset();
private:
    QSerialPort *mserial;
    void delay(uint32_t ms);
    QByteArray cmd_packet(uint8_t tag, uint8_t param_cnt, uint32_t *param, uint32_t time_wait_for_rx = 20);
};

#endif // KBOOT_PROTOCOL_H
