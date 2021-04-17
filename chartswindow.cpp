#include "chartswindow.h"
#include "ui_chartswindow.h"
#include <QDebug>

int ChartsWindow::acc_i = 0;
int ChartsWindow::mag_i = 0;
int ChartsWindow::gyro_i = 0;

ChartsWindow::ChartsWindow(QPointer<IMUCalc> imuCalc, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChartsWindow)
{
    ui->setupUi(this);
    this->imuCalc = imuCalc;

    QPointer<QLineSeries> series1 = new QLineSeries();
    QPointer<QLineSeries> series2 = new QLineSeries();
    QPointer<QLineSeries> series3 = new QLineSeries();
    QPointer<QLineSeries> series4 = new QLineSeries();
    QPointer<QLineSeries> series5 = new QLineSeries();
    QPointer<QLineSeries> series6 = new QLineSeries();
    QPointer<QLineSeries> series7 = new QLineSeries();
    QPointer<QLineSeries> series8 = new QLineSeries();
    QPointer<QLineSeries> series9 = new QLineSeries();

    QPointer<QChart> chart1 = new QChart();
    QPointer<QChart> chart2 = new QChart();
    QPointer<QChart> chart3 = new QChart();
    qchart.append(chart1);
    qchart.append(chart2);
    qchart.append(chart3);

    qchart.at(0)->legend()->hide();
    qchart.at(0)->addSeries(series1);
    qchart.at(0)->addSeries(series2);
    qchart.at(0)->addSeries(series3);
    qchart.at(0)->createDefaultAxes();
    qchart.at(0)->setTitle("Accelerometer");
    qchart.at(0)->axisX()->setTitleText("Index");
    qchart.at(0)->axisY()->setTitleText("g");

    qchart.at(1)->legend()->hide();
    qchart.at(1)->addSeries(series4);
    qchart.at(1)->addSeries(series5);
    qchart.at(1)->addSeries(series6);
    qchart.at(1)->createDefaultAxes();
    qchart.at(1)->setTitle("Gyroscope");
    qchart.at(1)->axisX()->setTitleText("Index");
    qchart.at(1)->axisY()->setTitleText("deg/sec");

    qchart.at(2)->legend()->hide();
    qchart.at(2)->addSeries(series7);
    qchart.at(2)->addSeries(series8);
    qchart.at(2)->addSeries(series9);
    qchart.at(2)->createDefaultAxes();
    qchart.at(2)->setTitle("Magnetometer");
    qchart.at(2)->axisX()->setTitleText("Index");
    qchart.at(2)->axisY()->setTitleText("gauss/sec");

    qchartView = new QChartView(qchart.at(0));
    qchartView->setRenderHint(QPainter::Antialiasing);
    qchartView->setChart(qchart.at(0));
    ui->gridLayout_2->addWidget(qchartView);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=]() {
        bool error = false;
        Acc acc;
        try {
            acc = imuCalc->getAccData().at(acc_i);
        }  catch (const std::out_of_range& e) {
            error = true;
        }

        if(!error)
        {
            series1->append(acc_i, acc.X);
            series2->append(acc_i, acc.Y);
            series3->append(acc_i, acc.Z);
            qchart.at(0)->axisX()->setRange((acc_i/200)*200, (acc_i/200+1)*200);
            acc_i++;

            if(acc_max < acc.X)
                acc_max = acc.X;

            if(acc_max < acc.Y)
                acc_max = acc.Y;

            if(acc_max < acc.Z)
                acc_max = acc.Z;

            if(acc_min > acc.X)
                acc_min = acc.X;

            if(acc_min > acc.Y)
                acc_min = acc.Y;

            if(acc_min > acc.Z)
                acc_min = acc.Z;

            qchart.at(0)->axisY()->setRange(acc_min, acc_max);
        }

        error = false;
        Gyro gyro;
        try {
            gyro = imuCalc->getGyroData().at(gyro_i);
        }  catch (const std::out_of_range& e) {
            error = true;
        }

        if(!error)
        {
            series4->append(gyro_i, gyro.X);
            series5->append(gyro_i, gyro.Y);
            series6->append(gyro_i, gyro.Z);
            qchart.at(1)->axisX()->setRange((gyro_i/200)*200, (gyro_i/200+1)*200);
            gyro_i++;

            if(gyro_max < gyro.X)
                gyro_max = gyro.X;

            if(gyro_max < gyro.Y)
                gyro_max = gyro.Y;

            if(gyro_max < gyro.Z)
                gyro_max = gyro.Z;

            if(gyro_min > gyro.X)
                gyro_min = gyro.X;

            if(gyro_min > gyro.Y)
                gyro_min = gyro.Y;

            if(gyro_min > gyro.Z)
                gyro_min = gyro.Z;

            qchart.at(1)->axisY()->setRange(gyro_min, gyro_max);
        }

        error = false;
        Mag mag;
        try {
            mag = imuCalc->getMagData().at(mag_i);
        }  catch (const std::out_of_range& e) {
            error = true;
        }

        if(!error)
        {
            series7->append(mag_i, mag.X);
            series8->append(mag_i, mag.Y);
            series9->append(mag_i, mag.Z);

            qchart.at(2)->axisX()->setRange((mag_i/200)*200, (mag_i/200+1)*200);
            mag_i++;

            if(mag_max < mag.X)
                mag_max = mag.X;

            if(mag_max < mag.Y)
                mag_max = mag.Y;

            if(mag_max < mag.Z)
                mag_max = mag.Z;

            if(mag_min > mag.X)
                mag_min = mag.X;

            if(mag_min > mag.Y)
                mag_min = mag.Y;

            if(mag_min > mag.Z)
                mag_min = mag.Z;

            qchart.at(2)->axisY()->setRange(mag_min, mag_max);
        }
    });
    timer->start(1);

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(switchChart(int)));
}

void ChartsWindow::switchChart(int a)
{
    qchartView->setChart(qchart.at(a));
}

ChartsWindow::~ChartsWindow()
{
    delete ui;
}
