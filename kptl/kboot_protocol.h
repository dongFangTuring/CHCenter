#ifndef KBOOT_PROTOCOL_H
#define KBOOT_PROTOCOL_H

#include <QtCore>


class kboot_protocol:public QObject
{
    Q_OBJECT

public:
    kboot_protocol();
    ~kboot_protocol();

    void kboot_clear();
    /* general API */
    void delay(uint32_t ms);


    /* bl related */
    bool connect();
    bool download(QByteArray image, int start_addr, int retry = 2);
    int max_packet_size() const { return _max_packet_size; }
    int flash_size() const { return _flash_size;}
    int flash_sec_size() const { return _flash_sec_size;}
    QString sdid() const { return _sdid; }
    int ver_bugfix() const {return _bugfix;}
    int ver_minor() const {return _minor;}
    int ver_major() const {return _major;}

public slots:
    void slt_serial_read(QByteArray &ba);

signals:
    void sig_download_progress(int precent);    /* notify download progess */
    void sig_serial_send(QByteArray &ba);       /* when need to send serial data */
    void sig_frame_recv(QByteArray &ba);        /* notify when a kptl frame has been recvived */

private:

    /* bl section */
    int _max_packet_size;
    int _flash_size;
    int _flash_sec_size;
    QString _sdid;
    uint8_t _bugfix;
    uint8_t _minor;
    uint8_t _major;

    /* rx state */
    int state;              /* receive state machine */
    QByteArray rx_raw;
    int rx_feame_len;
    QByteArray rx_payload;
    void decode(QByteArray &brx);
    bool resp_flag;


    bool serial_send_and_wait_resp(QByteArray &tx, int timeout = 1000);
    bool cmd_packet(uint8_t tag, uint8_t param_cnt, uint32_t *param);
    bool cmd_flash_erase_region(uint32_t addr, uint32_t len);
    bool cmd_flash_write_memory(uint32_t addr, uint32_t len);
    bool cmd_send_data_packet(QByteArray &buf);
    bool cmd_reset();
    void crc16(uint16_t *currectCrc, const uint8_t *src, uint32_t lengthInBytes);
    uint16_t do_crc_check(QByteArray &ba);
    QByteArray cmd_get_property(uint8_t property_code);

};

#endif // KBOOT_PROTOCOL_H
