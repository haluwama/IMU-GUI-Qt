#ifndef IMUCALC_H
#define IMUCALC_H

#include <QObject>
#include <QVector>
#include <vector>

struct Acc {
    float X, Y, Z;
};

struct Gyro {
    float X, Y, Z;
};

struct Mag {
    float X, Y, Z;
};

class IMUCalc : public QObject
{
    Q_OBJECT

public:
    explicit IMUCalc(QObject *parent = nullptr);
    void addDataAcc(QString, QString, QString);
    void addDataGyro(QString, QString, QString);
    void addDataMag(QString, QString, QString);

    std::vector<Acc> getAccData() {
        return AccData;
    }

    std::vector<Gyro> getGyroData() {
        return GyroData;
    }

    std::vector<Mag> getMagData() {
        return MagData;
    }

private:
    std::vector<Acc> AccData;
    std::vector<Gyro> GyroData;
    std::vector<Mag> MagData;

};

#endif // IMUCALC_H
