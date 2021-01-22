#include "kboot_protocol.h"

kboot_protocol::kboot_protocol(QSerialPort *mserial)
{
    this->mserial = mserial;

}

kboot_protocol::~kboot_protocol()
{

}

void kboot_protocol::delay(uint32_t ms)
{
    QEventLoop eventloop;
    QTimer::singleShot(ms, &eventloop, SLOT(quit()));
    eventloop.exec();
}

QByteArray kboot_protocol::serial_send_then_recv(QByteArray &tx, uint32_t ms)
{
    this->mserial->clear(QSerialPort::Input);
    //qDebug("serial_send_then_recv:" + tx.toHex(',') + '\n');
    this->mserial->write(tx);
    this->mserial->waitForBytesWritten();
    this->mserial->waitForReadyRead(100);
    delay(ms);
    return this->mserial->readAll();
}

bool kboot_protocol::ping(uint8_t &bugfix, uint8_t &minor, uint8_t &major)
{
    QByteArray ba;

    /* 5A A6 PING */
    packet_ping_t ping;
    kptl_create_ping(&ping);

    ba.clear();
    ba.append(reinterpret_cast<char *>(&ping), sizeof(packet_ping_t));
    ba = serial_send_then_recv(ba);


    if(ba.size() == 10)
    {
        uint8_t * buf = reinterpret_cast<uint8_t*>(ba.data());

        if(buf[0] == 0x5A && buf[1] == 0xA7)
        {
            bugfix = buf[2];
            minor  = buf[3];
            major  = buf[4];
            return true;
        }
    }
    return false;
}

QByteArray kboot_protocol::cmd_packet(uint8_t tag, uint8_t param_cnt, uint32_t *param, uint32_t time_wait_for_rx)
{
    kptl_t fp;
    cmd_hdr_t cp;

    cp.tag = tag;
    cp.param_cnt = param_cnt;
    cp.reserved = 0;
    cp.flags = 0;

    QByteArray ba;
    kptl_create_cmd_packet(&fp, &cp, param);
    ba.append((const char*)&fp, kptl_get_frame_size(&fp));
    return serial_send_then_recv(ba, time_wait_for_rx);
}


QByteArray kboot_protocol::get_property(uint8_t property_code)
{
    QByteArray ba;
    uint32_t tx_param = property_code;
    uint8_t cnt = 1;
    ba = cmd_packet(kCommandTag_GetProperty, cnt, &tx_param);
    if(ba.size() == 20)
    {
        ba.remove(0, 8);
        if ((uint8_t)ba.at(0) == 0xA7)
        {
            ba.remove(0, 8);
            return ba;
        }
    }
    ba.clear();
    return ba;
}

bool kboot_protocol::flash_erase_region(uint32_t addr, uint32_t len)
{
    QByteArray ba;

    uint32_t param[2];

    param[0] = addr;
    param[1] = len;
    ba = cmd_packet(kCommandTag_FlashEraseRegion, 2, param, 2000);
    //qDebug(ba.toHex(','));
    if(ba.size() == 20)
    {
        ba.remove(0, 8);
        if ((uint8_t)ba.at(0) == 0xA0)
        {
            return true;
        }

    }
    return false;
}

bool kboot_protocol::flash_write_memory(uint32_t addr, uint32_t len)
{
    QByteArray ba;
    uint32_t param[2];

    param[0] = addr;
    param[1] = len;
    ba = cmd_packet(kCommandTag_WriteMemory, 2, param);
    if(ba.size() == 20)
    {
        ba.remove(0, 8);
        if ((uint8_t)ba.at(0) == 0xA0)
        {
            return true;
        }
    }
    return false;
}

bool kboot_protocol::reset()
{
    cmd_packet(kCommandTag_Reset, 0, NULL);
    return true;
}



bool kboot_protocol::send_data_packet(QByteArray &buf, bool is_last)
{
    kptl_t fp;

    kptl_frame_packet_begin(&fp, kFramingPacketType_Data);
    kptl_frame_packet_add(&fp, buf.data(), buf.size());
    kptl_frame_packet_final(&fp);

    QByteArray ba;
    ba.append((const char*)&fp, kptl_get_frame_size(&fp));
    ba = serial_send_then_recv(ba, 40);
    //qDebug(ba.toHex(','));
    if(is_last)
    {
        if(ba.size() == 20)
        {
            return true;
        }
    }
    else
    {
        if(ba.size() == 2)
        {
            return true;
        }
    }

    return false;
}



