#include "baseform.h"

#include <QApplication>
#include <QTranslator>
#include <QMetaType>

QStringList loadsetting(QString setting){
    QStringList parameter = setting.split('=');
    return parameter;
}

/**
 * @brief main - set language before the window shows
 * language are defined with those abbreviation={"en","tc","cn"}
 *  * @return
 */

int main(int argc, char *argv[])
{

    QApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication a(argc, argv);

    //register type for passing value between signal/slot
    qRegisterMetaType<receive_imusol_packet_t>("receive_imusol_packet_t");
    qRegisterMetaType<receive_gwsol_packet_t>("receive_gwsol_packet_t");



    QSurfaceFormat fmt;
    fmt.setSamples(6);
    QSurfaceFormat::setDefaultFormat(fmt);

    a.setWindowIcon(QIcon(":/images/mainicon.png"));

    //load language setting
    QString lang="";
    QStringList setting={""};

    QFile file("CHCenter.ini");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        file.close();
        if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
            QTextStream stream(&file);
            stream << "Language=cn" <<"\n";
            file.close();
        }
    }
    else{
        QTextStream stream(&file);
        QString line;
        while (stream.readLineInto(&line)) {
            setting=loadsetting(line);
            if(setting.first()=="Language")
                lang=setting.last();
        }
        file.close();
    }

    QTranslator t;

    if(lang=="en") {
        t.load(":/CHCenter_en_US.qm");
    }
    else if (lang=="cn"){
        t.load(":/CHCenter_cn.qm");
    }
    else if (lang=="tc"){
        t.load(":/CHCenter_tc.qm");
    }
    else{
        t.load(":/CHCenter_cn.qm");
    }


    a.installTranslator(&t);

    BaseForm w;

    w.show();
    return a.exec();
}
