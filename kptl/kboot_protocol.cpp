#include "kboot_protocol.h"

/* processing bootloder protocol */

enum status
{
    kStatus_Idle,
    kStatus_Cmd,
    kStatus_LenLow,
    kStatus_LenHigh,
    kStatus_CRCLow,
    kStatus_CRCHigh,
    kStatus_Data,
};


/* packet type */
enum
{
    kFramingPacketStartByte         = 0x5A,
    kFramingPacketType_Ack          = 0xA1,
    kFramingPacketType_Nak          = 0xA2,
    kFramingPacketType_AckAbort     = 0xA3,
    kFramingPacketType_Command      = 0xA4,
    kFramingPacketType_Data         = 0xA5,
    kFramingPacketType_Ping         = 0xA6,
    kFramingPacketType_PingResponse = 0xA7
};

/* command tag */
enum
{
    kCommandTag_GenericResponse             = 0xa0,
    kCommandTag_FlashEraseAll               = 0x01,
    kCommandTag_FlashEraseRegion            = 0x02,
    kCommandTag_ReadMemory                  = 0x03,
    kCommandTag_ReadMemoryResponse          = 0xa3,
    kCommandTag_WriteMemory                 = 0x04,
    kCommandTag_FillMemory                  = 0x05,
    kCommandTag_FlashSecurityDisable        = 0x06,
    kCommandTag_GetProperty                 = 0x07,
    kCommandTag_GetPropertyResponse         = 0xa7,
    kCommandTag_ReceiveSbFile               = 0x08,
    kCommandTag_Execute                     = 0x09,
    kCommandTag_Call                        = 0x0a,
    kCommandTag_Reset                       = 0x0b,
    kCommandTag_SetProperty                 = 0x0c,
    kCommandTag_FlashEraseAllUnsecure       = 0x0d,
    kCommandTag_FlashProgramOnce            = 0x0e,
    kCommandTag_FlashReadOnce               = 0x0f,
    kCommandTag_FlashReadOnceResponse       = 0xaf,
    kCommandTag_FlashReadResource           = 0x10,
    kCommandTag_FlashReadResourceResponse   = 0xb0,
    kCommandTag_ConfigureQuadSpi            = 0x11,

    kFirstCommandTag                    = kCommandTag_FlashEraseAll,

    //! Maximum linearly incrementing command tag value, excluding the response commands.
    kLastCommandTag                     = kCommandTag_ConfigureQuadSpi,

    kResponseCommandHighNibbleMask = 0xa0           //!< Mask for the high nibble of a command tag that identifies it as a response command.
};

kboot_protocol::kboot_protocol()
{
    this->state = kStatus_Idle;
    this->rx_feame_len = 0;
    this->resp_flag = false;
}

kboot_protocol::~kboot_protocol()
{

}

void kboot_protocol::kboot_clear()
{
    this->state = kStatus_Idle;
    this->rx_feame_len = 0;
    this->resp_flag = false;
}

void kboot_protocol::decode(QByteArray &ba)
{
//    if(ba.size())
//    {
//        qDebug()<<"ktpl rx:"<<ba.toHex(',');
//    }

    for(int i =0; i<ba.size(); i++)
    {
        uint8_t c = ba.at(i);
        this->rx_payload.append(c);
        switch (this->state)
        {
        case kStatus_Idle:
            if(c == kFramingPacketStartByte)
            {
                this->rx_payload.clear();
                this->rx_payload.append(c);
                this->state = kStatus_Cmd;
            }
            break;
        case kStatus_Cmd:
            switch(c)
            {
            case kFramingPacketType_Command:
                this->state = kStatus_LenLow;
                break;
            case kFramingPacketType_Data:
                this->state = kStatus_LenLow;
                break;
            case kFramingPacketType_Ping:
                this->resp_flag = true;
                this->state = kStatus_Idle;
                break;
            case kFramingPacketType_PingResponse:
                this->rx_feame_len = (8+2);
                this->resp_flag = true;
                this->state = kStatus_Data;
                break;
            case kFramingPacketType_Ack:
            case kFramingPacketType_Nak:
                this->state = kStatus_Idle;
                break;
            }
            break;
        case kStatus_LenLow:
            this->state = kStatus_LenHigh;
            break;
        case kStatus_LenHigh:
            this->state = kStatus_CRCLow;
            break;
        case kStatus_CRCLow:
            this->state = kStatus_CRCHigh;
            break;
        case kStatus_CRCHigh:
            this->rx_feame_len  = ((uint8_t)this->rx_payload.at(2) | ((uint8_t)this->rx_payload.at(3)<<8)) + 6;

            if(this->rx_feame_len < 2048)
            {
                this->state = kStatus_Data;
            }
            else
            {
                this->state = kStatus_Idle;
            }

            break;
        case kStatus_Data:
            if(this->rx_payload.size() == this->rx_feame_len)
            {
                //qDebug()<<"kptl rx:"<<rx_payload.toHex(',');

                /* CRC */
                uint16_t crc_calculated = do_crc_check(rx_payload);
                uint8_t *p = reinterpret_cast<uint8_t*>(rx_payload.data());
                uint16_t crc_save = (p[4] | (p[5]<<8));

                /* CRC match */
                if(crc_calculated == crc_save)
                {
                    if((uint8_t)this->rx_payload[1] == (uint8_t)0xA5)
                    {
                        QByteArray out = this->rx_payload.remove(0, 6);
                        //qDebug()<<"kptl emit:"<<out.toHex(',');
                        emit sig_frame_recv(out);

                    }

                    this->resp_flag = true;
                }

                this->state = kStatus_Idle;
            }

            break;
        }

    }

}

void kboot_protocol::slt_serial_read(QByteArray &ba)
{
    this->rx_raw.append(ba);
    this->decode(this->rx_raw);
    this->rx_raw.clear();
}

void kboot_protocol::delay(uint32_t ms)
{
    QEventLoop eventloop;
    QTimer::singleShot(ms, &eventloop, SLOT(quit()));
    eventloop.exec();
}

/* expected_len: if recv data count within timeout == expected_len, return:true */
bool kboot_protocol::serial_send_and_wait_resp(QByteArray &tx, int timeout)
{
    emit sig_serial_send(tx);
    this->resp_flag = false;
    while(!resp_flag && timeout)
    {
        this->delay(1);
        timeout--;
    }

    return this->resp_flag;
}

bool kboot_protocol::cmd_packet(uint8_t tag, uint8_t param_cnt, uint32_t *param)
{
    int i;
    uint8_t rev = 0;
    uint8_t flags = 0;

    QByteArray ba;
    ba.resize(6);
    ba[0] = (uint8_t)0x5A;
    ba[1] = (uint8_t)0xA4; /* CMD packet */
    ba.append(QByteArray((const char*)&tag, 1));
    ba.append(QByteArray((const char*)&flags, 1));
    ba.append(QByteArray((const char*)&rev, 1));
    ba.append(QByteArray((const char*)&param_cnt, 1));

    for(i=0; i<param_cnt; i++)
    {
        ba.append(QByteArray((const char*)&param[i], 4));
    }

    /* add size */
    uint32_t payload_sz = ba.size() - 6;
    ba[2] = ((payload_sz>>0) & 0xFF);
    ba[3] = ((payload_sz>>8) & 0xFF);

    /* mark CRC */
    uint16_t crc = do_crc_check(ba);
    ba[4] = ((crc>>0) & 0xFF);
    ba[5] = ((crc>>8) & 0xFF);

    return serial_send_and_wait_resp(ba);

   // qDebug()<<"cmd_packet rx_payload:"<<this->rx_payload.size();
   // return this->rx_payload;
}


QByteArray kboot_protocol::cmd_get_property(uint8_t property_code)
{
    uint32_t tx_param = property_code;
    uint8_t param_cnt = 1;
    if(cmd_packet(kCommandTag_GetProperty, param_cnt, &tx_param))
    {
        this->rx_payload.remove(0, 6);
        this->rx_payload.remove(0, 8);
        return this->rx_payload;

    }

    return this->rx_payload;
}

bool kboot_protocol::cmd_flash_erase_region(uint32_t addr, uint32_t len)
{
    bool ret;
    uint32_t param[2];

    param[0] = addr;
    param[1] = len;
    ret = cmd_packet(kCommandTag_FlashEraseRegion, 2, param);
    return ret;
}

bool kboot_protocol::cmd_flash_write_memory(uint32_t addr, uint32_t len)
{
    uint32_t param[2];

    param[0] = addr;
    param[1] = len;
    return  cmd_packet(kCommandTag_WriteMemory, 2, param);

}

bool kboot_protocol::cmd_reset()
{
    cmd_packet(kCommandTag_Reset, 0, NULL);
    return true;
}

uint16_t kboot_protocol::do_crc_check(QByteArray &ba)
{
    uint16_t crc = 0;
    uint8_t *p = reinterpret_cast<uint8_t*>(ba.data());
    this->crc16(&crc, &p[0], 4);
    this->crc16(&crc, &p[6], ba.size() - 6);
    return crc;
}


bool kboot_protocol::cmd_send_data_packet(QByteArray &buf)
{
    QByteArray ba;

    ba.resize(6);
    ba[0] = (uint8_t)0x5A;
    ba[1] = (uint8_t)0xA5; /* CMD packet */
    ba.append(buf);

    /* add size */
    uint32_t payload_sz = ba.size() - 6;
    ba[2] = ((payload_sz>>0) & 0xFF);
    ba[3] = ((payload_sz>>8) & 0xFF);

    /* mark CRC */
    uint16_t crc = do_crc_check(ba);
    ba[4] = ((crc>>0) & 0xFF);
    ba[5] = ((crc>>8) & 0xFF);

    //qDebug(ba.toHex(','));


    emit sig_serial_send(ba);
    this->resp_flag = false;
    this->rx_payload.clear();

    uint32_t timeout = 200;
    while(timeout)
    {
        if((uint8_t)this->rx_payload.at(0) == (uint8_t)0x5A && (uint8_t)this->rx_payload.at(1) == (uint8_t)0xA1)
        {
            delay(10);
            return true;
        }

        this->delay(1);
        timeout--;
    }

    qDebug()<<"timeout!!"<<rx_payload.size()<<rx_payload.toHex(',');
    return false;
}


bool kboot_protocol::connect()
{
    QByteArray ba;

    /* 5A A6 PING */
    ba.resize(2);
    ba[0] = (uint8_t)0x5A;
    ba[1] = (uint8_t)0xA6;

    serial_send_and_wait_resp(ba);

    if(resp_flag)
    {
        uint8_t * buf = reinterpret_cast<uint8_t*>(this->rx_payload.data());
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
    ba = cmd_get_property(0x01);
    if(ba.size() == 4)
    {
        this->_major = ba.at(2);
        this->_minor = ba.at(1);
        this->_bugfix = ba.at(0);
    }
    else
        return false;

    /* get max packet size */
    ba = cmd_get_property(0x0B);
    if(ba.size() == 4)
    {
        this->_max_packet_size = (ba.at(0) | (ba.at(1)<<8) | ba.at(2)<<16 | ba.at(3)<<24);
    }
    else return false;

    /* flash size */
    ba = this->cmd_get_property(0x04);
    if(ba.size() == 4)
    {
        this->_flash_size = (ba.at(0) | (ba.at(1)<<8) | ba.at(2)<<16 | ba.at(3)<<24) / 1024;
    }
    else return false;

    //SDID
    ba = this->cmd_get_property(0x10);
    if(ba.size() == 4)
    {
        this->_sdid = ba.toHex();
    }
    else return false;

    //Flash Sector Size
    ba = this->cmd_get_property(0x05);
    if(ba.size() == 4)
    {
        this->_flash_sec_size = (ba.at(0) | (ba.at(1)<<8) | ba.at(2)<<16 | ba.at(3)<<24);
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
            if(cmd_send_data_packet(blob))
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

    delay(50);
    cmd_reset();
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
