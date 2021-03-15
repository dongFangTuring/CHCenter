#include "csvlogform.h"
#include "ui_csvlogform.h"

CSVLogForm::CSVLogForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CSVLogForm)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("CSV Logger"));

    //set a timer to record a section of time
    timer_log_period=new QTimer(this);
    connect(timer_log_period, SIGNAL(timeout()), this, SLOT(stopLogging()),Qt::QueuedConnection);
    timer_log_period->setSingleShot(true);

    //set a count down timer to start
    timer_countdown=new QTimer(this);
    connect(timer_countdown, SIGNAL(timeout()), this, SLOT(logging_countdown()),Qt::QueuedConnection);
    timer_countdown->setSingleShot(false);


    current_dir=QDir::currentPath()+"/log/data.csv";

    ui->LinePath->setText(current_dir);
    ui->SBCountDown->setValue(3);

    log_started=0;
    frame_counter=0;

    ui->BTNStart->setEnabled(1);
    ui->BTNStop->setEnabled(0);

    ui->SBLogPeriod->setRange(0,86400);
    ui->SBLogPeriod->setValue(10);
    ui->SBCountDown->setRange(0,86400);
    ui->SBCountDown->setValue(3);

    ui->textBrowser->append(tr("Make sure HI221GW and wireless nodes are in signal range avoid any loss of data."));
    ui->textBrowser->append(tr("The recorded data will be the same with data protocol."));
    ui->textBrowser->append(tr("If the frame rate are set higher than 100Hz, "
                               "you should increase Baud from 115200 to 460800 or 921600."));


}

CSVLogForm::~CSVLogForm()
{
    timer_log_period->stop();
    ch_logfile.close();
    delete ui;
}

void CSVLogForm::closeEvent(QCloseEvent *event)
{

    if(log_started) {
        QMessageBox msgBox;
        msgBox.setText(tr("You're still recording data"));
        msgBox.setInformativeText(tr("Do you really want to stop and exit?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        msgBox.setButtonText(QMessageBox::Yes, tr("Yes"));
        msgBox.setButtonText(QMessageBox::No, tr("No"));

        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Yes:
            stopLogging();
            event->accept();
            break;
        case QMessageBox::No:
            event->ignore();
            break;
        default:
            event->ignore();
            break;

        }


    }
}
void CSVLogForm::on_BTNPath_clicked()
{
    QString dir_path = QFileDialog::getSaveFileName(this, tr("Save File"),
                       current_dir,
                       tr("csv file (*.csv)"));

    if(!dir_path.isEmpty())
        current_dir=dir_path;

    ui->LinePath->setText(current_dir);
}

void CSVLogForm::on_BTNStart_clicked()
{
    current_dir=ui->LinePath->text();


    QDate dt_now = QDate::currentDate();
    QTime tm_now = QTime::currentTime();

    //check if a directory "log" exists, if not,
    //create a new folder.
    if(!QDir("log").exists())
        QDir().mkdir("log");

    //add timestamp
    QString timestamp="_"+dt_now.toString("yyyyMMdd") + tm_now.toString("_hhmmss");
    QString temp=current_dir;
    QString fullpath=temp.replace(".csv",timestamp+".csv");

    //update remaining time label
    int remainMsec=ui->SBLogPeriod->value()*1000;
    QTime time(0,0,0);
    time=time.addMSecs(remainMsec);
    QString str_time=time.toString("hh:mm:ss.zzz");
    ui->LabelRemainTime->setText(str_time);

    frame_counter=0;
    ui->LabelFrameCounter->setText("0");

    int index1=fullpath.lastIndexOf("/");
    int index2=fullpath.lastIndexOf(".csv");

    temp=fullpath;
    QString dir=temp.remove(index1,index2-index1+4);

    bool dir_exists = QFileInfo::exists(dir) && QFileInfo(dir).isDir();
    if(!dir_exists==1) {

        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error"));
        msgBox.setText(tr("The save directory not exists."));

        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok, tr("OK"));
        msgBox.exec();
    } else {

        if(log_started==1) {
            ui->BTNStart->setEnabled(0);
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Wait..."));
            msgBox.setText(tr("Already logging!"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setButtonText(QMessageBox::Ok, tr("OK"));
            msgBox.exec();
        } else {
            //message
            ui->textBrowser->append(tr("->Save path : %1").arg(fullpath));

            ch_logfile.setFileName(fullpath);
            ui->BTNStart->setEnabled(0);

            if(ui->CBCountDown->isChecked()) {
                timer_countdown->setInterval(1000);
                timer_countdown->start();
                countdown_sec=ui->SBCountDown->value();
            } else {
                ui->BTNStop->setEnabled(1);
                startLogging();
            }
        }
    }

}


void CSVLogForm::on_BTNStop_clicked()
{
    stopLogging();
}

void CSVLogForm::on_BTNClear_clicked()
{
    ui->textBrowser->clear();
}

void CSVLogForm::getIMUData(id0x91_t imu_data)
{

    if(log_started==1) {

        int remainMsec=timer_log_period->remainingTime();

        QTime time(0,0,0);
        time=time.addMSecs(remainMsec);
        QString str_time=time.toString("hh:mm:ss.zzz");
        ui->LabelRemainTime->setText(str_time);

        if(frame_counter==0) {
            if(!ch_logfile.open(QIODevice::WriteOnly| QIODevice::Append)) {
                stopLogging();
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Log error!"));
                msgBox.setText(tr("The current file can't be writed. "
                                  "Please close other windows that may using the file."));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setButtonText(QMessageBox::Ok, tr("Ok"));
                msgBox.exec();

            } else {
                QTextStream stream(&ch_logfile);
                stream << tr("Time,Frame,AccX,AccY,AccZ,GyrX,GyrY,GyrZ,"
                             "MagX,MagY,MagZ,Roll,Pitch,Yaw,Qw,Qx,Qy,Qz")<<"\n";
                frame_counter++;
            }

        } else {
            if(!ch_logfile.isOpen()) {
                qDebug()<<"not opened";
                ch_logfile.open(QIODevice::WriteOnly| QIODevice::Append);
            } else {
                QTextStream stream(&ch_logfile);

                QString tm_now = QTime::currentTime().toString("hh-mm-ss.zzz");

                QString csv_row=tr("%1,%2,").arg(tm_now).arg(frame_counter);
                csv_row+=imudata2csvrow(imu_data);

                int ret=csv_row.lastIndexOf(',');
                if(ret!=-1)
                    csv_row.remove(ret,1);

                stream << csv_row <<"\n";

                frame_counter++;
            }
        }
        ui->LabelFrameCounter->setText(QString::number(frame_counter-1));
    }

}
void CSVLogForm::getDongleData(QVector<id0x91_t> packets)
{
    //qDebug()<<frame_counter;
    if(log_started==1) {

        int remainMsec=timer_log_period->remainingTime();

        QTime time(0,0,0);
        time=time.addMSecs(remainMsec);
        QString str_time=time.toString("hh:mm:ss.zzz");
        ui->LabelRemainTime->setText(str_time);

        if(packets.size()>0) {

            if(frame_counter==0) {

                gwnode_idlist.clear();

                int remainMsec=timer_log_period->remainingTime();
                QTime time(0,0,0);
                time=time.addMSecs(remainMsec);
                QString str_time=time.toString("hh:mm:ss.zzz");
                ui->LabelRemainTime->setText(str_time);

                if(!ch_logfile.open(QIODevice::WriteOnly| QIODevice::Append)) {
                    stopLogging();
                    QMessageBox msgBox;
                    msgBox.setWindowTitle(tr("Log error!"));
                    msgBox.setText(tr("The current file can't be writed. "
                                      "Please close other windows that may using the file."));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setButtonText(QMessageBox::Ok, tr("OK"));
                    msgBox.exec();
                } else {
                    QTextStream stream(&ch_logfile);

                    stream << "Time,Frame,";

                    for(unsigned short i=0; i<packets.size(); i++) {
                        auto imu_data=packets.at(i);
                        gwnode_idlist.push_back(imu_data.id);

                        QString title_row=tr("AccX(id%1),AccY(id%1),AccZ(id%1),GyrX(id%1),GyrY(id%1),GyrZ(id%1),"
                                             "MagX(id%1),MagY(id%1),MagZ(id%1),Roll(id%1),Pitch(id%1),Yaw(id%1),"
                                             "Qw(id%1),Qx(id%1),Qy(id%1),Qz(id%1),").arg(imu_data.id);

                        if(i==packets.size()-1) {
                            int ret=title_row.lastIndexOf(',');
                            if(ret!=-1)
                                title_row.remove(ret,1);
                        }
                        stream << title_row;
                    }
                    stream << "\n";
                    frame_counter++;
                }
            } else {
                if(!ch_logfile.isOpen()) {
                    qDebug()<<"not opened";
                    ch_logfile.open(QIODevice::WriteOnly| QIODevice::Append);
                } else {
                    QTextStream stream(&ch_logfile);

                    QString tm_now = QTime::currentTime().toString("hh-mm-ss.zzz");
                    stream << tr("%1,%2,").arg(tm_now).arg(frame_counter);


                    //if we lost some nodes.
                    if(packets.size()<gwnode_idlist.size()) {
                        for(unsigned short i=0; i<gwnode_idlist.size(); i++) {

                            QString csv_row=",,,,,,,,,,,,,,,,";
                            for(int j=0; j<packets.size(); j++) {

                                auto imu_data=packets.at(j);

                                if(imu_data.id==gwnode_idlist[i]) {
                                    csv_row=imudata2csvrow(imu_data);
                                }
                            }

                            if(i==gwnode_idlist.size()-1) {
                                int ret=csv_row.lastIndexOf(',');
                                if(ret!=-1)
                                    csv_row.remove(ret,1);
                            }
                            stream<<csv_row;

                        }
                        stream <<"\n";
                        frame_counter++;
                    }
                    //if all nodes are online
                    else {

                        for(unsigned short i=0; i<packets.size(); i++) {
                            auto imu_data=packets.at(i);

                            QString csv_row=imudata2csvrow(imu_data);

                            if(i==packets.size()-1) {
                                int ret=csv_row.lastIndexOf(',');
                                if(ret!=-1)
                                    csv_row.remove(ret,1);
                            }

                            stream<<csv_row;
                        }

                        stream <<"\n";
                        frame_counter++;
                    }
                }
            }
            ui->LabelFrameCounter->setText(QString::number(frame_counter-1));
        }
    }

}
void CSVLogForm::getBitmap(uchar bitmap)
{
    m_bitmap=bitmap;
}

QString CSVLogForm::imudata2csvrow(id0x91_t imu_data)
{

    QString csv_row="";
    if(m_bitmap&BIT_VALID_TIME_STAMP) {
        csv_row=tr("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,")
                .arg(QString::number(imu_data.acc[0],'f',3))
                .arg(QString::number(imu_data.acc[1],'f',3))
                .arg(QString::number(imu_data.acc[2],'f',3))
                .arg(QString::number(imu_data.gyr[0],'f',3))
                .arg(QString::number(imu_data.gyr[1],'f',3))
                .arg(QString::number(imu_data.gyr[2],'f',3))
                .arg(QString::number(int(imu_data.mag[0])))
                .arg(QString::number(int(imu_data.mag[1])))
                .arg(QString::number(int(imu_data.mag[2])))
                .arg(QString::number(imu_data.eul[0],'f',2))
                .arg(QString::number(imu_data.eul[1],'f',2))
                .arg(QString::number(imu_data.eul[2],'f',2))
                .arg(QString::number(imu_data.quat[0],'f',3))
                .arg(QString::number(imu_data.quat[1],'f',3))
                .arg(QString::number(imu_data.quat[2],'f',3))
                .arg(QString::number(imu_data.quat[3],'f',3));
    } else {
        if(m_bitmap & BIT_VALID_ACC) {
            for(unsigned short i=0; i<3; i++) {
                csv_row+=QString::number(imu_data.acc[i],'f',3)+",";
            }
        } else {
            csv_row+=",,,";
        }
        if(m_bitmap & BIT_VALID_GYR) {
            for(unsigned short i=0; i<3; i++) {
                csv_row+=QString::number(imu_data.gyr[i],'f',3)+",";
            }
        } else {
            csv_row+=",,,";
        }
        if(m_bitmap & BIT_VALID_MAG) {
            for(unsigned short i=0; i<3; i++) {
                csv_row+=QString::number(int(imu_data.mag[i]))+",";
            }
        } else {
            csv_row+=",,,";
        }
        if(m_bitmap & BIT_VALID_EUL) {
            for(unsigned short i=0; i<3; i++) {
                csv_row+=QString::number(imu_data.eul[i],'f',3)+",";
            }
        } else {
            csv_row+=",,,";
        }
        if(m_bitmap & BIT_VALID_QUAT) {
            for(unsigned short i=0; i<4; i++) {
                csv_row+=QString::number(imu_data.quat[i],'f',3)+",";
            }
        } else {
            csv_row+=",,,,";
        }
    }

    return csv_row;


}



void CSVLogForm::on_SBLogPeriod_valueChanged(int arg1)
{
    int remainMsec=arg1*1000;

    QTime time(0,0,0);
    time=time.addMSecs(remainMsec);
    QString str_time=time.toString("hh:mm:ss.zzz");
    ui->LabelRemainTime->setText(str_time);

}



void CSVLogForm::startLogging()
{
    frame_counter=0;
    log_started=1;
    timer_log_period->setInterval(ui->SBLogPeriod->value()*1000);
    timer_log_period->start();
    ui->textBrowser->append(tr("->Start logging ......"));
    QTimer::singleShot(1000, this, SLOT(checkIsStarted()));
}

void CSVLogForm::stopLogging()
{
    ui->BTNStart->setEnabled(1);
    ui->BTNStop->setEnabled(0);

    log_started=0;
    on_SBLogPeriod_valueChanged(0);
    if(ch_logfile.isOpen()) {
        ui->textBrowser->append(tr("->File has been successfully saved at : %1\n").arg(ch_logfile.fileName()));
        ch_logfile.close();
    } else {
        ui->textBrowser->append(tr("->No file is saved! Please check connection, Baud, and frame rate setting."));
    }
    if(timer_log_period->isActive()) {
        timer_log_period->stop();
    }


}

void CSVLogForm::checkIsStarted()
{
    if(frame_counter==0) {
        stopLogging();
    } else {
        //keep
    }
}

void CSVLogForm::logging_countdown()
{

    if(countdown_sec<=0) {

        startLogging();
        ui->BTNStop->setEnabled(1);
        timer_countdown->stop();
    } else {
        ui->textBrowser->append(tr("->Will be started at %1s ...").arg(countdown_sec));
        countdown_sec--;
    }

}


