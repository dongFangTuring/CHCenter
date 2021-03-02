#include "mdbus.h"
#include <QtCore>

#define MB_FUNC_READ_INPUT_REGISTER           (  4 )
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS      ( 16 )
#define MB_FUNC_ERROR                         ( 128 )
#define MAX_PDU_SIZE                          ( 253 )

enum
{
    kStatus_Addr,
    kStatus_Funcode,
    kStatus_ByteCnt,
    kStatus_Data,
    kStatus_CRCHigh,
    kStatus_CRCLow,
};




mdbus::mdbus(QObject *parent) : QObject(parent)
{
    this->brx.clear();
    this->state = kStatus_Addr;
}

void mdbus::decode(QByteArray &brx)
{

    if(brx.size())
    {
        qDebug()<<"mdbus rx:"<<brx.toHex(',');
        //qDebug()<<"mdbus rx size:"<<brx.size();
    }

    for(int i =0; i<brx.size(); i++)
    {
        uint8_t c = brx.at(i);

        switch (this->state)
        {
        case kStatus_Addr:
            if(c == this->slv_addr)
            {
                this->rx_payload.clear();
                this->rx_payload.append(c);
                this->state = kStatus_Funcode;
            }
            break;

        case kStatus_Funcode:
            this->fun_code = c;
            rx_payload.append(c);
            switch(this->fun_code)
            {
            case MB_FUNC_READ_INPUT_REGISTER:
                this->state = kStatus_ByteCnt;
                break;

            case MB_FUNC_WRITE_MULTIPLE_REGISTERS:
                this->state = kStatus_Data;
                break;
            default:
                this->state = kStatus_Addr;
                break;
            }
            break;
        case kStatus_ByteCnt:
            this->rx_byte_cnt = c;
            rx_payload.append(c);
            if(this->rx_byte_cnt <= MAX_PDU_SIZE)
            {
                if(this->rx_byte_cnt)
                    this->state = kStatus_Data;
                else
                    this->state = kStatus_CRCLow;
                break;
            }
            else
            {
                this->state = kStatus_Addr;
            }
        case kStatus_Data:
            rx_payload.append(c);

            if(this->fun_code == MB_FUNC_READ_INPUT_REGISTER && rx_payload.size() == this->rx_byte_cnt + 3) /* 3 = addr(1) + fun_code(1) + byte_cnt(1) */
            {
                this->state = kStatus_CRCLow;
            }

            if(this->fun_code == MB_FUNC_WRITE_MULTIPLE_REGISTERS && rx_payload.size() == 6)
            {
                this->state = kStatus_CRCLow;
            }

            if(rx_payload.size() > MAX_PDU_SIZE) /* something err! */
            {
                qDebug()<<"mdbus decode err\n";
                this->state = kStatus_Addr;
            }
            break;
        case kStatus_CRCLow:
            this->recv_crc = c;
            this->state = kStatus_CRCHigh;
            break;
        case kStatus_CRCHigh:
            this->recv_crc |= c<<8;
            if(this->crc16(rx_payload) == this->recv_crc)
            {
                rx_payload.remove(0, 3); /* 3 = addr(1) + fun_code(1) + byte_cnt(1) */
                this->recv_ok = true;
            }
            else
            {
                qDebug()<<"mdbus crc err\n";
            }

            this->state = kStatus_Addr;
            break;
        }
    }

}

/*
  * dev_addr: 1-127 mdbus device addr, defalut 1
  * each REG is 4B
  * reg_addr: register addr 0:REG0  1:REG1
  * buf: pointer of REGs
  * len: number of register:  len=1(1 REG)  len=2:(2 REG)
 */
bool mdbus::read_data(uint8_t dev_addr, uint16_t reg_addr, uint32_t *buf, uint16_t len, uint32_t timeout)
{
    this->slv_addr = dev_addr;

    /* create req */
    QByteArray tba;
    tba.append(dev_addr);
    tba.append(MB_FUNC_READ_INPUT_REGISTER);
    tba.append(((reg_addr*2)>>8) & 0xFF);
    tba.append(((reg_addr*2)>>0) & 0xFF);
    tba.append(((len*2)>>8) & 0xFF);
    tba.append(((len*2)>>0) & 0xFF);

    /* add CRC */
    uint16_t crc = this->crc16(tba);
    tba.append((crc>>0) & 0xFF);
    tba.append((crc>>8) & 0xFF);


    //    qDebug()<<"mdbus tx:"<<tba.toHex(',');

    /* paepare for recv */
    sig_serial_send(tba);

    this->brx.clear();
    this->rx_payload.clear();
    this->serial_read_flag = true;
    this->recv_ok = false;

    /* wait for resp data or timeout */
    while(!this->recv_ok && timeout)
    {
        this->decode(brx);
        this->brx.clear();
        this->delay(1);
        timeout--;
    }

    this->serial_read_flag = false;

    if(this->recv_ok)
    {
        for(int i=0; i<this->rx_payload.size() / 4; i++)
        {
            QByteArray qval = this->rx_payload.mid(i*4, 4);
            uint32_t *val = reinterpret_cast<uint32_t*>(qval.data());
            buf[i] = qFromBigEndian(val[0]);
        }
    }

    return this->recv_ok;
}


/*
 * dev_addr: 1-127 mdbus device addr, default 1
 * each REG is 4B
 * reg_addr: register addr 0:REG0  1:REG1
 * buf: pointer of REGs
*/
bool mdbus:: write_data(uint8_t dev_addr, uint16_t reg_addr, uint32_t *buf, uint16_t len, uint32_t timeout)
{
    this->slv_addr = dev_addr;

    uint32_t val;
    QByteArray ba;

    /* create req */
    ba.append(dev_addr);
    ba.append(MB_FUNC_WRITE_MULTIPLE_REGISTERS);
    ba.append(((reg_addr*2)>>8) & 0xFF);
    ba.append(((reg_addr*2)>>0) & 0xFF);
    ba.append(((len*2)>>8) & 0xFF);
    ba.append(((len*2)>>0) & 0xFF);
    ba.append(len*4);

    for(int i=0; i<len; i++)
    {
        val = qToBigEndian(buf[i]);
        ba.append(QByteArray((const char*)&val, 4));
    }

    /* add CRC */
    uint16_t crc = this->crc16(ba);
    ba.append((crc>>0) & 0xFF);
    ba.append((crc>>8) & 0xFF);

    qDebug()<<"mdbus tx: "<<ba.toHex(',');

    emit sig_serial_send(ba);
    this->brx.clear();
    this->rx_payload.clear();
    this->serial_read_flag = true;
    this->recv_ok = false;

    /* wait for resp data or timeout */
    while(!this->recv_ok && timeout)
    {
        this->decode(brx);
        this->brx.clear();
        this->delay(1);
        timeout--;
    }

    this->serial_read_flag = false;

    return this->recv_ok;
}

/* warp into a single read read/write API */
bool mdbus::read_reg(uint8_t dev_addr, uint16_t reg_addr, uint32_t &val, uint32_t timeout)
{
    return read_data(dev_addr, reg_addr, &val, 1, timeout);
}

/* warp into a single read read/write API */
bool mdbus::write_reg(uint8_t dev_addr, uint16_t reg_addr, uint32_t val, uint32_t timeout)
{
    return write_data(dev_addr, reg_addr, &val, 1, timeout);
}


void mdbus::delay(uint32_t ms)
{
    QEventLoop eventloop;
    QTimer::singleShot(ms, &eventloop, SLOT(quit()));
    eventloop.exec();
}

void mdbus::slt_serial_read(QByteArray &ba)
{
    if(this->serial_read_flag)
    {
        this->brx.append(ba);
    }
}


uint16_t mdbus::crc16(QByteArray senddata)
{
    int len=senddata.size();
    uint16_t wcrc=0XFFFF;
    uint8_t temp;
    int i=0,j=0;
    for(i=0;i<len;i++)
    {
        temp=senddata.at(i);
        wcrc^=temp;
        for(j=0;j<8;j++){

            if(wcrc&0X0001){
                wcrc>>=1;
                wcrc^=0XA001;
            }
            else
                wcrc>>=1;
        }
    }
    temp = wcrc;
    return wcrc;
}

