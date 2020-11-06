#include "baseform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BaseForm w;
    w.show();
    return a.exec();
}
