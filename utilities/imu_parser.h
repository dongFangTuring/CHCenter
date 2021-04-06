#ifndef IMU_PARSER_H
#define IMU_PARSER_H

#include <QObject>


typedef struct
{
    uint8_t     tag;                /* 0x91 */
    uint8_t     id;
    int16_t     pos[3];              /* HP pos, used for short time posation tracking, cannot used for navigation */
    uint32_t    ts;                 /* timestamp */
    float       acc[3];
    float       gyr[3];
    float       mag[3];
    float       eul[3];             /* eular angles:R/P/Y */
    float       quat[4];            /* quaternion */
}id0x91_t;

typedef struct
{
    uint8_t         tag;            /* 0x62 */
    uint8_t         gwid;           /* network ID */
    uint8_t         node_cnt;       /* number of RF device */
    uint8_t         rev[5];
}id0x62_hdr_t;



class imu_parser : public QObject
{
    Q_OBJECT

#define BIT_VALID_ID          (0x01)
#define BIT_VALID_ACC         (0x02)
#define BIT_VALID_GYR         (0x04)
#define BIT_VALID_MAG         (0x08)
#define BIT_VALID_EUL         (0x10)
#define BIT_VALID_QUAT        (0x20)
#define BIT_VALID_TIME_STAMP  (0x40)
#define BIT_RF_DONGLE         (0x80)

#define MAX_NODE_SIZE 16




public:
    explicit imu_parser(QObject *parent = nullptr);
    uint32_t bitmap;


    void parse(QByteArray &ba);

    id0x91_t        dev[MAX_NODE_SIZE];       /* dev[0] is filled when single IMU connected */
    id0x62_hdr_t    dev_info;                 /* device information */

    uint8_t item_id[32];
    uint8_t item_id_cnt;

signals:

};

#endif // IMU_PARSER_H
