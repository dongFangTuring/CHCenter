#ifndef MDBUS_H
#define MDBUS_H

#include <QObject>

class mdbus : public QObject
{
    Q_OBJECT
public:
    explicit mdbus(QObject *parent = nullptr);
    bool read_data(uint8_t dev_addr, uint16_t reg_addr, uint32_t *buf, uint16_t len, uint32_t timeout = 100); /* read multiple REG, max REG requst cnt must < 16 */
    bool write_data(uint8_t dev_addr, uint16_t reg_addr, uint32_t *buf, uint16_t len, uint32_t timeout = 100);/* write multiple REG, max REG requst cnt must < 16 */
    bool read_reg(uint8_t dev_addr, uint16_t reg_addr, uint32_t &val, uint32_t timeout = 100);  /* write signle REG */
    bool write_reg(uint8_t dev_addr, uint16_t reg_addr, uint32_t val, uint32_t timeout = 100);  /* write signle REG */
    void delay(uint32_t ms);


public slots:
    void slt_serial_read(QByteArray &ba);  /* slt for recv data */

signals:
    void sig_serial_send(QByteArray &ba); /* sig for send data */

private:
    int state;              /* receive state machine */
    uint8_t slv_addr;       /* current req slv addr */
    uint8_t fun_code;       /* current req fun code */
    uint8_t rx_byte_cnt;    /* rx data byte cnt */
    uint16_t recv_crc;      /* recved frame's CRC */
    bool recv_ok;           /* if a frame is recv succ */
    bool serial_read_flag;   /* if or not recv data from serial slt */
    QByteArray brx;         /* serial recv buf */
    QByteArray rx_payload;
    uint16_t crc16(QByteArray senddata);
    void decode(QByteArray &brx);
};

#endif // MDBUS_H
