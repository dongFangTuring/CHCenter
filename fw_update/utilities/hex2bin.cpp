#include "hex2bin.h"



uint32_t hex2bin::ExStageAddr = 0x00;//扩展段地址
uint32_t hex2bin::ExLineAddr = 0x00;//扩展线性地址
uint32_t hex2bin::absoluteAddrLocal = 0x00;//本地记录绝对地址
uint32_t hex2bin::_start_addr = 0xFFFFFFFF;


bool hex2bin::read_hex_line(line_data_t *out, const QByteArray &ba)//false: 校验错误 true：校验成功
{
    unsigned char i,checkoutCal=0;

    //计算校验值
    for(i=0;i < ba.size()-1;i++){
        checkoutCal += (unsigned char)ba.at(i);
    }
    checkoutCal = 0x100-checkoutCal;
    //获取个部分域的值
    out->datalen =(unsigned char)ba.at(0);
    out->addr = ((unsigned char)ba.at(1)<<8)|(unsigned char)ba.at(2);
    out->datatype = (unsigned char)ba.at(3);
    memset(out->databuf,0,sizeof(out->databuf));
    for(i = 0;i<out->datalen;i++){
        out->databuf[i] = (unsigned char)ba.at(4+i);
    }
    out->checkout = (unsigned char)ba.at(4+i);
#if  0 //调试时打开
    qDebug("datalen=%X",out->datalen);
    qDebug("addr=%X",out->addr);
    qDebug("datatype=%X",out->datatype);
    qDebug("checkout=%X",out->checkout);
    qDebug("checkoutCal=%X",checkoutCal);
#endif
    //比较读取的校验值和计算的校验值是否一致
    if(checkoutCal == out->checkout){
        return true;
    }
    return false;
}


uint32_t hex2bin::start_addr(void)
{
    return hex2bin::_start_addr;
}


char hex2bin::hex_to_bin(line_data_t* ba, QByteArray &out)//return 0: ok  1:hex文件结束 2：hex文件有误
{

    unsigned int absoluteAddrCurrent = 0x00;//计算当前记录的绝对地址
    unsigned int Bytesskipped = 0;//被跳过的字节数

    switch(ba->datatype){
    case 0x00://数据记录
        //计算出当前记录的绝对地址
        if(ExStageAddr != 0){
            absoluteAddrCurrent = (ba->addr+ExStageAddr);
        }else if(ExLineAddr != 0){
            absoluteAddrCurrent = (ba->addr|ExLineAddr);
        }else{
            absoluteAddrCurrent = ba->addr;
        }

        if(_start_addr == 0xFFFFFFFF)
        {
            _start_addr = absoluteAddrCurrent;
        }
        //hex文件第一条数据记录时，将本地绝对地址absoluteAddrLocal同步等于当前记录的绝对地址absoluteAddrCurrent
        if(absoluteAddrLocal == 0){
            absoluteAddrLocal = absoluteAddrCurrent;
        }
        Bytesskipped = absoluteAddrCurrent-absoluteAddrLocal;//比较当前记录的绝对地址absoluteAddrCurrent和本地的绝对地址absoluteAddrLocal是否有偏差
        break;
    case 0x01://文件结束记录
        return 1;
        break;
    case 0x02://扩展段地址记录
        ExStageAddr = (ba->databuf[0]<<8|ba->databuf[1])<<2;
        ExLineAddr = 0x00;
        return 0;//return ok
        break;
    case 0x04://扩展线性地址记录
        ExLineAddr = (ba->databuf[0]<<8|ba->databuf[1])<<16;
        ExStageAddr = 0x00;
        return 0;//return ok
        break;
    default:
        return 2;
        break;
    }

    for(unsigned int i = 0;i < Bytesskipped;i++){//被跳过的地址，填充0
        out.append(QByteArray::number(0));
    }
    if(Bytesskipped!=0){
        qDebug() <<Bytesskipped;
    }
    absoluteAddrLocal += Bytesskipped;//本地绝对地址absoluteAddrLocal累加

    for(unsigned int i = 0;i < ba->datalen;i++){
        out.append(ba->databuf[i]);

    }
    absoluteAddrLocal += ba->datalen;//本地绝对地址absoluteAddrLocal累加

    return 0;//return ok
}



QByteArray hex2bin::convert(QTextStream &ts)
{
    QByteArray ba_line;
    QByteArray ba_out;
    line_data_t HexDataStr;

    ExStageAddr = 0x00;//扩展段地址
    ExLineAddr = 0x00;//扩展线性地址
    absoluteAddrLocal = 0x00;//本地记录绝对地址

    while(1)
    {
        ba_line = QByteArray::fromHex(ts.readLine().toUtf8());//从hex文件中读取一行

        if(ba_line.size() > 0)
        {
            bool ret = read_hex_line(&HexDataStr, ba_line);//将一行数据解读到HexDataStr结构体
            if(!ret){
                qDebug("校验出错,hex文件有误.");
                ba_out.clear();
                return ba_out;
            }
            ret = hex_to_bin(&HexDataStr, ba_out);//将解读后的数据写入bin文件
            if(ret != 0)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return ba_out;
}
