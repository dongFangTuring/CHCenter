#ifndef HEX2BIN_H
#define HEX2BIN_H

#include <QtCore>

class hex2bin:public QObject
{
    Q_OBJECT

private:

     typedef struct{
        unsigned char datalen;//数据字节
        unsigned short addr;//地址域
        unsigned char datatype;//类型
        unsigned char databuf[16];//数据记录
        unsigned char checkout;//校验和
    }line_data_t ;

    static bool read_hex_line(line_data_t* out,const QByteArray & ba);
    static char hex_to_bin(line_data_t* ba, QByteArray &out);

    static uint32_t ExStageAddr;
    static uint32_t ExLineAddr;
    static uint32_t absoluteAddrLocal;
    static uint32_t _start_addr;
public:
    static QByteArray convert(QTextStream &hex_text);
    static uint32_t start_addr();
};

#endif // HEX2BIN_H
