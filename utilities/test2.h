#ifndef TEST2_H
#define TEST2_H

#include <QtCore>


class test2:public QObject
{
    Q_OBJECT

    public:
        test2();
        test2(int a);

        float aa = 5;
        float get_x() {return 3;}
//        void send()
//        {
//            emit newPaper("dwasds");
//        }



signals:
    void sig_test();


};

#endif // TEST2_H
