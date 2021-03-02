#include "kboot_protocol.h"

/* processing bootloder protocol */

static uint8_t resp_cnt = 0;

void kptl_callback(kptl_t *pkt)
{
    Q_UNUSED(pkt);

    resp_cnt++;
}

kboot_protocol::kboot_protocol()
{
    static kptl_t rx_pkt;
    this->dec.pkt = &rx_pkt;
    this->dec.cb = kptl_callback;
    kptl_decode_init(&this->dec);

}

kboot_protocol::~kboot_protocol()
{

}

void kboot_protocol::slt_serial_read(QByteArray &ba)
{
    this->brx.append(ba);
}

void kboot_protocol::delay(uint32_t ms)
{
    QEventLoop eventloop;
    QTimer::singleShot(ms, &eventloop, SLOT(quit()));
    eventloop.exec();
}

/* expected_len: if recv data count within timeout == expected_len, return:true */
bool kboot_protocol::serial_send_then_recv(QByteArray &tx, QByteArray &rx, int expected_len, int timeout)
{
    emit sig_serial_send(tx);

    rx.clear();
    brx.clear();

    while(this->brx.size() < expected_len && timeout)
    {
        delay(1);
        timeout--;
    }

    rx.append(brx);
    return (rx.size() == expected_len);
}


QByteArray kboot_protocol::cmd_packet(uint8_t tag, uint8_t param_cnt, uint32_t *param, int expected_len)
{
    int i;
    uint8_t rev = 0;
    uint8_t flags = 0;

    QByteArray btx;
    btx.resize(6);
    btx[0] = 0x5A;
    btx[1] = 0xA4; /* CMD packet */
    btx.append(QByteArray((const char*)&tag, 1));
    btx.append(QByteArray((const char*)&flags, 1));
    btx.append(QByteArray((const char*)&rev, 1));
    btx.append(QByteArray((const char*)&param_cnt, 1));

    for(i=0; i<param_cnt; i++)
    {
        btx.append(QByteArray((const char*)&param[i], 4));
    }

    /* add size */
    uint32_t payload_sz = btx.size() - 6;
    btx[2] = ((payload_sz>>0) & 0xFF);
    btx[3] = ((payload_sz>>8) & 0xFF);

    uint16_t crc = 0;

    QByteArray qcrc = btx;
    qcrc.remove(4,2);
    uint8_t *crc_cont = reinterpret_cast<uint8_t*>(qcrc.data());
    this->crc16(&crc, crc_cont, qcrc.size());
    btx[4] = ((crc>>0) & 0xFF);
    btx[5] = ((crc>>8) & 0xFF);

//    qDebug()<<btx.toHex(',');
//    qDebug()<<"CRC"<<crc;

    QByteArray brx;
    brx.clear();

    serial_send_then_recv(btx, brx, expected_len);

    /* check if data recv ok */
    resp_cnt = 0;
    for(int i=0; i<brx.size(); i++)
    {
        kptl_decode(&this->dec, brx.at(i));
    }

    if(resp_cnt == 0)
    {
        brx.clear();
    }

    return brx;
}


QByteArray kboot_protocol::cmd_get_property(uint8_t property_code)
{
    QByteArray brx;
    uint32_t tx_param = property_code;
    uint8_t param_cnt = 1;
    brx = cmd_packet(kCommandTag_GetProperty, param_cnt, &tx_param, 20);
    if(brx.size() == 20)
    {
        brx.remove(0, 8);
        brx.remove(0, 8);
        return brx;
    }

    brx.clear();
    return brx;
}

bool kboot_protocol::cmd_flash_erase_region(uint32_t addr, uint32_t len)
{
    QByteArray ba;

    uint32_t param[2];

    param[0] = addr;
    param[1] = len;
    ba = cmd_packet(kCommandTag_FlashEraseRegion, 2, param, 20);
    return (ba.size() == 20);

}

bool kboot_protocol::cmd_flash_write_memory(uint32_t addr, uint32_t len)
{
    QByteArray ba;
    uint32_t param[2];

    param[0] = addr;
    param[1] = len;
    ba = cmd_packet(kCommandTag_WriteMemory, 2, param, 20);
    return (ba.size() == 20);

}

bool kboot_protocol::cmd_reset()
{
    cmd_packet(kCommandTag_Reset, 0, NULL, 2);
    return true;
}

void kboot_protocol::crc16(uint16_t *currectCrc, const uint8_t *src, uint32_t lengthInBytes)
{
    uint32_t crc = *currectCrc;
    uint32_t j;
    for (j=0; j < lengthInBytes; ++j)
    {
        uint32_t i;
        uint32_t byte = src[j];
        crc ^= byte << 8;
        for (i = 0; i < 8; ++i)
        {
            uint32_t temp = crc << 1;
            if (crc & 0x8000)
            {
                temp ^= 0x1021;
            }
            crc = temp;
        }
    }
    *currectCrc = crc;
}

bool kboot_protocol::cmd_send_data_packet(QByteArray &buf, bool is_last)
{
    QByteArray ba;

    ba.resize(6);
    ba[0] = 0x5A;
    ba[1] = 0xA5; /* CMD packet */
    ba.append(buf);

    /* add size */
    uint32_t payload_sz = ba.size() - 6;
    ba[2] = ((payload_sz>>0) & 0xFF);
    ba[3] = ((payload_sz>>8) & 0xFF);

    uint16_t crc = 0;

    QByteArray qcrc = ba;
    qcrc.remove(4,2);
    uint8_t *crc_cont = reinterpret_cast<uint8_t*>(qcrc.data());
    this->crc16(&crc, crc_cont, qcrc.size());
    ba[4] = ((crc>>0) & 0xFF);
    ba[5] = ((crc>>8) & 0xFF);


    //qDebug(ba.toHex(','));
    if(is_last)
    {
        serial_send_then_recv(ba, ba, 20);
        if(ba.size() == 20)
        {
            return true;
        }
    }
    else
    {
        serial_send_then_recv(ba, ba, 2);
        if(ba.size() == 2)
        {
            return true;
        }
    }

    return false;
}




bool kboot_protocol::connect()
{
    QByteArray btx, brx;

    /* 5A A6 PING */
    btx.resize(2);
    btx[0] = 0x5A;
    btx[1] = 0xA6;

    serial_send_then_recv(btx, brx, 10);

    /* check if data recv ok */
    resp_cnt = 0;
    for(int i=0; i<brx.size(); i++)
    {
        kptl_decode(&this->dec, brx.at(i));
    }

    if(resp_cnt)
    {
        uint8_t * buf = reinterpret_cast<uint8_t*>(brx.data());
        if(buf[0] == 0x5A && buf[1] == 0xA7)
        {
            _bugfix = buf[2];
            _minor  = buf[3];
            _major  = buf[4];
        }
        else
        {
            return false;
        }
    }

    /* version */
    brx = cmd_get_property(0x01);
    if(brx.size() == 4)
    {
        this->_major = brx.at(2);
        this->_minor = brx.at(1);
        this->_bugfix = brx.at(0);
    }
    else
        return false;

    /* get max packet size */
    brx = cmd_get_property(0x0B);
    if(brx.size() == 4)
    {
        this->_max_packet_size = (brx.at(0) | (brx.at(1)<<8) | brx.at(2)<<16 | brx.at(3)<<24);
    }
    else return false;

    /* flash size */
    brx = this->cmd_get_property(0x04);
    if(brx.size() == 4)
    {
        this->_flash_size = (brx.at(0) | (brx.at(1)<<8) | brx.at(2)<<16 | brx.at(3)<<24) / 1024;
    }
    else return false;

    //SDID
    brx = this->cmd_get_property(0x10);
    if(brx.size() == 4)
    {
        this->_sdid = brx.toHex();
    }
    else return false;

    //Flash Sector Size
    brx = this->cmd_get_property(0x05);
    if(brx.size() == 4)
    {
        this->_flash_sec_size = (brx.at(0) | (brx.at(1)<<8) | brx.at(2)<<16 | brx.at(3)<<24);
    }
    else return false;

    return true;
}



bool kboot_protocol::download(QByteArray image, int start_addr, int retry)
{

    if(!cmd_flash_erase_region(start_addr, image.size()))
    {
        return false;
    }

    int sz = image.size();
    int i = 0;

    if(!cmd_flash_write_memory(start_addr, image.size()))
    {
        return false;
    }

    while(i < sz)
    {
        int pkt_len = (sz - i) > _max_packet_size?(this->_max_packet_size):(sz - 1);
        QByteArray blob = image.mid(i, pkt_len);

        while(retry)
        {
            if(cmd_send_data_packet(blob, (blob.size() == this->_max_packet_size)?(false):(true)))
            {
                i += blob.size();
                break;
            }
            else
            {
                retry--;
            }
        }

        /* retry many times, return failed */
        if(retry == 0)
        {
            return false;
        }
        emit sig_download_progress(i*100 / sz);
    }

    cmd_reset();
    return true;
}




