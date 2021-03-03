#ifndef IMU_PARSER_H
#define IMU_PARSER_H

#include <QObject>

class imu_parser : public QObject
{
    Q_OBJECT
public:
    explicit imu_parser(QObject *parent = nullptr);

signals:

};

#endif // IMU_PARSER_H
