#ifndef __KPTL_H__
#define __KPTL_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PACKET_LEN          (2048)


#if !defined(ARRAY2INT16)
#define ARRAY2INT16(x)     (x[0] + (x[1] << 8))
#endif


/* header include start_byte and type */
typedef struct
{
    uint8_t start_byte;
    uint8_t packet_type;
}packet_hr_t;

typedef struct
{
    packet_hr_t     hr;
    uint8_t         len[2];
    uint8_t         crc16[2];
    uint8_t         payload[MAX_PACKET_LEN];
}kptl_t;

typedef struct
{
    kptl_t*             pkt;
    uint32_t            cnt;
    void                (*cb)(kptl_t *pkt);
    uint8_t             status;
}pkt_dec_t;


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





#ifdef __cplusplus
}
#endif

#endif

