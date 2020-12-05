#include "csvlogform.h"
#include "ui_csvlogform.h"

CSVLogForm::CSVLogForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CSVLogForm)
{
    ui->setupUi(this);

    timer_log_period=new QTimer(this);
    connect(timer_log_period, SIGNAL(timeout()), this, SLOT(stopLogging()),Qt::QueuedConnection);
    timer_log_period->setSingleShot(true);

    timer_countdown=new QTimer(this);
    connect(timer_countdown, SIGNAL(timeout()), this, SLOT(logging_countdown()),Qt::QueuedConnection);
    timer_countdown->setSingleShot(false);


    current_path=QDir::currentPath()+"/log";
    dir=current_path;

    QDate dt_now = QDate::currentDate();
    QTime tm_now = QTime::currentTime();

    QString filename="chlog_"+dt_now.toString("yyyy-MM-dd") + tm_now.toString("_hh-mm-ss")+".csv";

    ui->LinePath->setText(dir+"/"+filename);
    ui->SBCountDown->setValue(3);

    log_started=0;
    frame_counter=0;

    ui->BTNStart->setEnabled(1);
    ui->BTNPause->setEnabled(0);
    ui->BTNStop->setEnabled(0);

    ui->SBLogPeriod->setRange(0,86400);
    ui->SBLogPeriod->setValue(10);

}

CSVLogForm::~CSVLogForm()
{
    timer_log_period->stop();
    ch_logfile.close();
    delete ui;
}

void CSVLogForm::on_BTNPath_clicked()
{

    QString dirtemp = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                        current_path,
                                                        QFileDialog::DontResolveSymlinks);
    if(!dirtemp.isEmpty())
        dir=dirtemp;

    QDate dt_now = QDate::currentDate();
    QTime tm_now = QTime::currentTime();

    QString filename="chlog_"+dt_now.toString("yyyy-MM-dd") + tm_now.toString("_hh-mm-ss")+".csv";

    ui->LinePath->setText(dir+"/"+filename);


}

void CSVLogForm::on_BTNSync_clicked()
{
    QDate dt_now = QDate::currentDate();
    QTime tm_now = QTime::currentTime();

    QString filename="chlog_"+dt_now.toString("yyyy-MM-dd") + tm_now.toString("_hh-mm-ss")+".csv";

    ui->LinePath->setText(dir+"/"+filename);
}

void CSVLogForm::on_BTNStart_clicked()
{
    if(log_started==1){
        ui->BTNStart->setEnabled(0);
    }
    else{

        filename_logging=ui->LinePath->text();
        ch_logfile.setFileName(filename_logging);

        ui->BTNStart->setEnabled(0);
    }
    if(ui->CBCountDown->isChecked()){
        timer_countdown->setInterval(1000);
        timer_countdown->start();
        countdown_sec=ui->SBCountDown->value();
    }
    else{
        startLogging();
        ui->BTNPause->setEnabled(1);
        ui->BTNStop->setEnabled(1);
    }
}
void CSVLogForm::getIMUData(receive_imusol_packet_t imu_data)
{

    if(log_started==1){

        int remainMsec=timer_log_period->remainingTime();

        QTime time(0,0,0);
        time=time.addMSecs(remainMsec);
        QString str_time=time.toString("hh:mm:ss.zzz");
        ui->LabelRemainTime->setText(str_time);

        if(frame_counter==0){
            if(!ch_logfile.open(QIODevice::WriteOnly| QIODevice::Append)){

                QMessageBox msgBox;
                msgBox.setText(tr("Log error!"));
                msgBox.setInformativeText(tr("The current file can't be writed. \n"
                                             "Please close other windows that may using the file."));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setButtonText(QMessageBox::Ok, tr("Ok"));
                stopLogging();

            }
            else{
                QTextStream stream(&ch_logfile);
                stream << tr("Time,Frame Counter,AccX,AccY,AccZ,GyrX,GyrY,GyrZ,"
                             "MagX,MagY,MagZ,Roll,Pitch,Yaw,Qw,Qx,Qy,Qz")<<"\n";
                frame_counter++;
            }

        }
        else{
            if(!ch_logfile.isOpen()){
                qDebug()<<"not opened";
                ch_logfile.open(QIODevice::WriteOnly| QIODevice::Append);
            }
            else{
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
    }
    else{
        if(ch_logfile.isOpen()){
            ch_logfile.close();
            qDebug()<<"not logging";
        }
    }

}
void CSVLogForm::getGWIMUData(receive_gwsol_packet_t gwimu_data)
{
    if(log_started==1){

        int remainMsec=timer_log_period->remainingTime();

        QTime time(0,0,0);
        time=time.addMSecs(remainMsec);
        QString str_time=time.toString("hh:mm:ss.zzz");
        ui->LabelRemainTime->setText(str_time);

        if(gwimu_data.n>0){

            if(frame_counter==0){

                gwnode_idlist.clear();

                int remainMsec=timer_log_period->remainingTime();
                QTime time(0,0,0);
                time=time.addMSecs(remainMsec);
                QString str_time=time.toString("hh:mm:ss.zzz");
                ui->LabelRemainTime->setText(str_time);

                if(!ch_logfile.open(QIODevice::WriteOnly| QIODevice::Append)){

                    QMessageBox msgBox;
                    msgBox.setText(tr("Log error!"));
                    msgBox.setInformativeText(tr("The current file can't be writed. \n"
                                                 "Please close other windows that may using the file."));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setButtonText(QMessageBox::Ok, tr("OK"));
                    stopLogging();

                }
                else{
                    QTextStream stream(&ch_logfile);

                    stream << "Time,Frame Counter,";

                    for(unsigned short i=0;i<gwimu_data.n;i++){
                        auto imu_data=gwimu_data.receive_imusol[i];
                        gwnode_idlist.push_back(imu_data.id);

                        QString title_row=tr("AccX(id%1),AccY(id%1),AccZ(id%1),GyrX(id%1),GyrY(id%1),GyrZ(id%1),"
                                             "MagX(id%1),MagY(id%1),MagZ(id%1),Roll(id%1),Pitch(id%1),Yaw(id%1),"
                                             "Qw(id%1),Qx(id%1),Qy(id%1),Qz(id%1),").arg(imu_data.id);

                        if(i==gwimu_data.n-1){
                            int ret=title_row.lastIndexOf(',');
                            if(ret!=-1)
                                title_row.remove(ret,1);
                        }
                        stream << title_row;
                    }
                    stream << "\n";
                    frame_counter++;
                }
            }
            else{
                if(!ch_logfile.isOpen()){
                    qDebug()<<"not opened";
                    ch_logfile.open(QIODevice::WriteOnly| QIODevice::Append);
                }
                else{
                    QTextStream stream(&ch_logfile);

                    QString tm_now = QTime::currentTime().toString("hh-mm-ss.zzz");
                    stream << tr("%1,%2,").arg(tm_now).arg(frame_counter);


                    //if we lost some nodes.
                    if(gwimu_data.n<gwnode_idlist.size()){
                        for(unsigned short i=0;i<gwnode_idlist.size();i++){

                            QString csv_row=",,,,,,,,,,,,,,,,";
                            for(int j=0;j<gwimu_data.n;j++){

                                auto imu_data=gwimu_data.receive_imusol[j];

                                if(imu_data.id==gwnode_idlist[i]){
                                    csv_row=imudata2csvrow(imu_data);
                                }
                            }

                            if(i==gwnode_idlist.size()-1){
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
                    else{

                        for(unsigned short i=0;i<gwimu_data.n;i++){
                            auto imu_data=gwimu_data.receive_imusol[i];

                            QString csv_row=imudata2csvrow(imu_data);



                            if(i==gwimu_data.n-1){
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
        }
    }

    else{
        stopLogging();

    }
}
void CSVLogForm::getBitmap(unsigned int bitmap)
{
    m_bitmap=bitmap;
}

QString CSVLogForm::imudata2csvrow(receive_imusol_packet_t imu_data)
{

    QString csv_row="";
    if(m_bitmap==BIT_VALID_ALL){
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
    }
    else{
        if(m_bitmap & BIT_VALID_ACC){
            for(unsigned short i=0;i<3;i++){
                csv_row+=QString::number(imu_data.acc[i],'f',3)+",";
            }
        }
        else{
            csv_row+=",,,";
        }
        if(m_bitmap & BIT_VALID_GYR){
            for(unsigned short i=0;i<3;i++){
                csv_row+=QString::number(imu_data.gyr[i],'f',3)+",";
            }
        }
        else{
            csv_row+=",,,";
        }
        if(m_bitmap & BIT_VALID_MAG){
            for(unsigned short i=0;i<3;i++){
                csv_row+=QString::number(int(imu_data.mag[i]))+",";
            }
        }
        else{
            csv_row+=",,,";
        }
        if(m_bitmap & BIT_VALID_EUL){
            for(unsigned short i=0;i<3;i++){
                csv_row+=QString::number(imu_data.eul[i],'f',3)+",";
            }
        }
        else{
            csv_row+=",,,";
        }
        if(m_bitmap & BIT_VALID_QUAT){
            for(unsigned short i=0;i<4;i++){
                csv_row+=QString::number(imu_data.quat[i],'f',3)+",";
            }
        }
        else{
            csv_row+=",,,,";
        }
    }

    return csv_row;


}

void CSVLogForm::on_BTNStop_clicked()
{
    stopLogging();
}

void CSVLogForm::on_BTNPause_clicked()
{

    if(log_started==1){
        log_started=0;
        ui->BTNPause->setText("Continue");
        int remaining=timer_log_period->remainingTime();
        timer_log_period->stop();
        timer_log_period->setInterval(remaining);

        if(ch_logfile.isOpen())
            ch_logfile.close();
    }
    else if(log_started==0){
        if(!ch_logfile.isOpen())
            ch_logfile.open(QIODevice::WriteOnly| QIODevice::Append);
        log_started=1;
        ui->BTNPause->setText("Pause");
        timer_log_period->start();
    }
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
}

void CSVLogForm::stopLogging()
{
    log_started=0;
    on_SBLogPeriod_valueChanged(0);
    if(ch_logfile.isOpen())
    {
        ch_logfile.close();
    }
    if(timer_log_period->isActive()){
        timer_log_period->stop();
    }

    ui->BTNStart->setEnabled(1);
    ui->BTNPause->setEnabled(0);
    ui->BTNStop->setEnabled(0);
}

void CSVLogForm::logging_countdown()
{
    QTime time(0,0,0);
    time=time.addSecs(countdown_sec);
    QString str_time=time.toString("hh:mm:ss");
    ui->LabelRemainTime->setText(str_time);

    if(countdown_sec<=0){

        startLogging();
        ui->BTNPause->setEnabled(1);
        ui->BTNStop->setEnabled(1);
        timer_countdown->stop();
    }
    else
        countdown_sec--;
}
