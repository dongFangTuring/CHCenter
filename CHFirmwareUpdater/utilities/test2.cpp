#include "test2.h"

test2::test2()
{
    qDebug("test2 called\r\n");
}

class Point {
public:
    double x,y;
    Point():x(0.0), y(0.0) {}
    ~Point() {int a = 5;};
};


test2::test2(int a)
{
    qDebug("test2(int a) called\r\n");

    emit sig_test();
}

