#ifndef IMU_PARSER_H
#define IMU_PARSER_H

#include <QObject>

class imu_parser : public QObject
{
    Q_OBJECT

    #define BIT_VALID_ID     (0x01)
    #define BIT_VALID_ACC    (0x02)
    #define BIT_VALID_GYR    (0x04)
    #define BIT_VALID_MAG    (0x08)
    #define BIT_VALID_EUL    (0x10)
    #define BIT_VALID_QUAT   (0x20)
    #define BIT_VALID_TIME_STAMP  (0x40)
    #define BIT_VALID_ALL  (BIT_VALID_QUAT | BIT_VALID_EUL | BIT_VALID_MAG | BIT_VALID_GYR | BIT_VALID_ACC | BIT_VALID_ID | BIT_VALID_TIME_STAMP)

    #define MAX_NODE_SIZE 16

    typedef struct
    {
        uint8_t     tag;                /* data packet tag */
        uint8_t     id;
        uint8_t     rev[6];             /* reserved */
        uint32_t    ts;                 /* timestamp */
        float       acc[3];
        float       gyr[3];
        float       mag[3];
        float       eul[3];             /* eular angles:R/P/Y */
        float       quat[4];            /* quaternion */
    }id0x91_t;

    typedef struct
    {
        uint8_t         tag;
        uint8_t         gwid;
        uint8_t         node_cnt;
        uint8_t         rev[5];
        id0x91_t        node[MAX_NODE_SIZE];
    }id0x62_t;


public:
    explicit imu_parser(QObject *parent = nullptr);
    uint8_t bitmap;
    void parse(QByteArray &ba);

    id0x91_t    dev;                /* single node dev */
    id0x62_t    rf;                 /* rf device node */
signals:

};

#endif // IMU_PARSER_H
