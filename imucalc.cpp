#include "imucalc.h"
#include <QDebug>

IMUCalc::IMUCalc(QObject *parent) : QObject(parent)
{
}

void IMUCalc::addDataAcc(QString a, QString b, QString c)
{

    Acc acc;
    acc.X = a.toFloat();
    acc.Y = b.toFloat();
    acc.Z = c.toFloat();

    AccData.push_back(acc);
}

void IMUCalc::addDataGyro(QString a, QString b, QString c)
{
    Gyro gyro;
    gyro.X = a.toFloat();
    gyro.Y = b.toFloat();
    gyro.Z = c.toFloat();

    GyroData.push_back(gyro);
}

void IMUCalc::addDataMag(QString a, QString b, QString c)
{
    Mag mag;
    mag.X = a.toFloat();
    mag.Y = b.toFloat();
    mag.Z = c.toFloat();

    MagData.push_back(mag);
}
