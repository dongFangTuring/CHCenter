#include "baseform.h"

#include <QApplication>
#include <QTranslator>


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
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/mainicon.png"));
    QString lang="";
    QStringList setting={""};

    QFile file("CHCenter.ini");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox msgBox;
        msgBox.setText("No CHCenter.ini was found.");
        msgBox.setWindowTitle("Error");
        msgBox.exec();

    }
    else{
        QTextStream stream(&file);
        QString line;
        while (stream.readLineInto(&line)) {
            setting=loadsetting(line);
            if(setting.first()=="Language")
                lang=setting.last();
        }
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
