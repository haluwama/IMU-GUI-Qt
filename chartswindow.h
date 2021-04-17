#ifndef CHARTSWINDOW_H
#define CHARTSWINDOW_H

#include <QMainWindow>
#include <QChart>
#include <QChartView>
#include <QPointer>
#include <QLineSeries>
#include <QTimer>
#include "imucalc.h"

namespace Ui {
class ChartsWindow;
}

using namespace QtCharts;

class ChartsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChartsWindow(QPointer<IMUCalc> imuCalc, QWidget *parent = nullptr);
    ~ChartsWindow();

private slots:
    void switchChart(int);
private:
    Ui::ChartsWindow *ui;
    QPointer<IMUCalc> imuCalc;
    QVector<QPointer<QChart>> qchart;
    QPointer<QChartView> qchartView;
    QVector<QPointer <QLineSeries>> series;
    QPointer <QTimer> timer;
    static int acc_i, mag_i, gyro_i;
    float acc_max = 0.0f, acc_min = 0.0f,
          gyro_max = 0.0f, gyro_min = 0.0f,
          mag_max = 0.0f, mag_min = 0.0f;
};

#endif // CHARTSWINDOW_H
