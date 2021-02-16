#include "test2.h"

test2::test2()
{
    qDebug("test2 called\r\n");
}

test2::test2(int a)
{
    qDebug("test2(int a) called\r\n");

    emit sig_test();
    this->send();
}

