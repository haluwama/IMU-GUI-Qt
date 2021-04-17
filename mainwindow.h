#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPointer>
#include <QSerialPort>
#include <QLabel>
#include "imucalc.h"
#include "chartswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readDataFromPort();

private:
    void addLog(QString);
    QPointer<IMUCalc> imuCalc;
    QPointer<QLabel> labelIcon;
    QPointer<QLabel> labelStatus;
    QPointer<QSerialPort> serialPort;
    QPointer<ChartsWindow> chartsWindow;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
