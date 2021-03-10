#include "imu_parser.h"
#include <QDebug>
#include <string.h>
#include <stdio.h>

typedef enum
{
    kItemID =                   0x90,   /* user programed ID   */
    kItemAccRaw =               0xA0,   /* raw acc             */
    kItemGyrRaw =               0xB0,   /* raw gyro            */
    kItemMagRaw =               0xC0,   /* raw mag             */
    kItemRotationEul =          0xD0,   /* eular               */
    kItemRotationQuat =         0xD1,   /* quat                */
    kItemPressure =             0xF0,   /* pressure            */
    KItemIMUSOL =               0x91,   /* IMUSOL  */
    KItemDongle =               0x62,   /* RFSOL  */
    KItemDongleRaw =            0x63,   /* RF Raw acc&gyro, suggest to discard  */
    kItemEnd =                  0x00,
}ItemID_t;


imu_parser::imu_parser(QObject *parent) : QObject(parent)
{
    this->item_id_cnt = 0;
}

static int stream2int16(int *dest,uint8_t *src)
{
    dest[0] = (int16_t)(src[0] | src[1] << 8);
    dest[1] = (int16_t)(src[2] | src[3] << 8);
    dest[2] = (int16_t)(src[4] | src[5] << 8);
    return 0;
}

void imu_parser::parse(QByteArray &ba)
{
    int temp[3] = {0};
    int offset = 0;
    int len = ba.size();
    bitmap = 0;

    item_id_cnt = 0;
    uint8_t *p = reinterpret_cast<uint8_t*>(ba.data());

    for(int i =0; i<len; i++)
    {
        while(offset < len)
        {
            switch(p[offset])
            {
            case kItemID:
                item_id[item_id_cnt++] = p[offset];
                bitmap |= BIT_VALID_ID;
                this->dev[0].id = p[1];
                offset += 2;
                break;

            case kItemAccRaw:
                item_id[item_id_cnt++] = p[offset];
                bitmap |= BIT_VALID_ACC;
                stream2int16(temp, &p[offset + 1]);
                this->dev[0].acc[0] = (float)temp[0] / 1000;
                this->dev[0].acc[1] = (float)temp[1] / 1000;
                this->dev[0].acc[2] = (float)temp[2] / 1000;
                offset += 7;
                break;

            case kItemGyrRaw:
            case 0xB1:
                item_id[item_id_cnt++] = p[offset];
                bitmap |= BIT_VALID_GYR;
                stream2int16(temp, &p[offset + 1]);
                this->dev[0].gyr[0] = (float)temp[0] / 10;
                this->dev[0].gyr[1] = (float)temp[1] / 10;
                this->dev[0].gyr[2] = (float)temp[2] / 10;
                offset += 7;
                break;

            case kItemMagRaw:
                item_id[item_id_cnt++] = p[offset];
                bitmap |= BIT_VALID_MAG;
                stream2int16(temp, &p[offset + 1]);
                this->dev[0].mag[0] = (float)temp[0] / 10;
                this->dev[0].mag[1] = (float)temp[1] / 10;
                this->dev[0].mag[2] = (float)temp[2] / 10;
                offset += 7;
                break;

            case kItemRotationEul:
                item_id[item_id_cnt++] = p[offset];
                bitmap |= BIT_VALID_EUL;
                stream2int16(temp, &p[offset + 1]);
                this->dev[0].eul[1] = (float)temp[0] / 100;
                this->dev[0].eul[0] = (float)temp[1] / 100;
                this->dev[0].eul[2] = (float)temp[2] / 10;
                offset += 7;
                break;

            case kItemRotationQuat:
                item_id[item_id_cnt++] = p[offset];
                bitmap |= BIT_VALID_QUAT;
                memcpy((void*)this->dev[0].quat, p + offset + 1, sizeof(this->dev[0].quat));
                offset += 17;
                break;

            case kItemPressure:
                item_id[item_id_cnt++] = p[offset];
                offset += 5;
                break;

            case KItemIMUSOL:
                item_id[item_id_cnt++] = p[offset];
                bitmap |= (BIT_VALID_QUAT | BIT_VALID_EUL | BIT_VALID_MAG | BIT_VALID_GYR | BIT_VALID_ACC | BIT_VALID_ID | BIT_VALID_TIME_STAMP);
                memcpy(&this->dev[0], &p[offset], sizeof(id0x91_t));
                offset += sizeof(id0x91_t);
                break;

            case 0x60:
                offset += (4*8)+1;
                break;
            case KItemDongle:
                item_id[item_id_cnt++] = p[offset];
                bitmap |= (BIT_RF_DONGLE | BIT_VALID_QUAT | BIT_VALID_EUL | BIT_VALID_MAG | BIT_VALID_GYR | BIT_VALID_ACC | BIT_VALID_ID | BIT_VALID_TIME_STAMP);

                /* fill rf hdr */
                memcpy(&this->dev_info, &p[offset], 8);
                offset += 8;

                /* fill each node */
                for (int i=0; i<this->dev_info.node_cnt; i++)
                {
                    memcpy(&this->dev[i], &p[offset], sizeof(id0x91_t));
                    offset += sizeof(id0x91_t);
                }
                break;

            case KItemDongleRaw:
                //                receive_gwsol.tag = KItemDongleRaw;
                //                receive_gwsol.gw_id = p[offset + 1];
                //                receive_gwsol.n = p[offset + 2];
                //                offset += 8;
                //                bitmap |= BIT_VALID_ID;
                //                bitmap |= BIT_VALID_ACC;
                //                bitmap |= BIT_VALID_GYR;

                //                for (int i = 0; i < receive_gwsol.n; i++)
                //                {

                //                    receive_gwsol.receive_imusol[i].tag = p[offset];
                //                    receive_gwsol.receive_imusol[i].id = p[offset + 1];

                //                    offset += 12;
                //                    stream2int16(temp, p + offset);
                //                    receive_gwsol.receive_imusol[i].acc[0] = (float)temp[0] / 1000;
                //                    receive_gwsol.receive_imusol[i].acc[1] = (float)temp[1] / 1000;
                //                    receive_gwsol.receive_imusol[i].acc[2] = (float)temp[2] / 1000;
                //                    offset += 6;
                //                    stream2int16(temp, p + offset);
                //                    receive_gwsol.receive_imusol[i].gyr[0] = (float)temp[0] / 10;
                //                    receive_gwsol.receive_imusol[i].gyr[1] = (float)temp[1] / 10;
                //                    receive_gwsol.receive_imusol[i].gyr[2] = (float)temp[2] / 10;
                //                    offset += 6;
                //                }
                break;

            default:
                offset++;
            }
        }
    }


}
