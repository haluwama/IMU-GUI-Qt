#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QRandomGenerator>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    labelIcon = new QLabel();
    labelIcon->setText("     ");
    labelIcon->setStyleSheet("background-color: red");
    ui->statusbar->addWidget(labelIcon);

    labelStatus = new QLabel();
    labelStatus->setText("No connection");
    ui->statusbar->addWidget(labelStatus);

    serialPort = new QSerialPort();

    imuCalc = new IMUCalc();
    chartsWindow = new ChartsWindow(imuCalc);

    connect(ui->pushButtonShowWindowCharts, &QPushButton::clicked, this, [=]() {
        chartsWindow->show();
    });

    connect(ui->pushButtonSearch, &QPushButton::clicked, this, [=] () {
        if(ui->comboBoxDevicesList->count() > 0)
                ui->comboBoxDevicesList->clear();

            const auto serialPortInfos = QSerialPortInfo::availablePorts();

            if(serialPortInfos.length() > 0)
            {
                for (const QSerialPortInfo &serialPortInfo : serialPortInfos)
                {
                    ui->comboBoxDevicesList->addItem(serialPortInfo.portName() + "\t" + serialPortInfo.description());
                }
            }
            else
            {
                QMessageBox::about(this, "Attention", "There isn\'t any device connected to USB");
            }
    });

    connect(ui->pushButtonConnect, &QPushButton::clicked, this, [=] () {
        if(ui->comboBoxDevicesList->count() == 0)
            {
                addLog("There isn't any device!");
                return;
            }
            if(!serialPort->isOpen())
            {
                QString portName = ui->comboBoxDevicesList->currentText().split("\t").first();
                serialPort->setPortName(portName);

                if(serialPort->open(QSerialPort::ReadWrite))
                {
                    serialPort->setBaudRate(QSerialPort::Baud38400);
                    serialPort->setDataBits(QSerialPort::Data8);
                    serialPort->setParity(QSerialPort::NoParity);
                    serialPort->setStopBits(QSerialPort::TwoStop);
                    serialPort->setFlowControl(QSerialPort::NoFlowControl);

                    connect(serialPort, SIGNAL(readyRead()), this, SLOT(readDataFromPort()));
                    labelIcon->setStyleSheet("background-color: green");
                    labelStatus->setText(ui->comboBoxDevicesList->currentText());
                    ui->pushButtonConnect->setText("Close");
                    ui->pushButtonSearch->setEnabled(false);
                    ui->comboBoxDevicesList->setEnabled(false);
                    ui->checkBoxSaveData->setEnabled(false);

                    addLog("Opened a communication");
                }
                else
                {
                    addLog("There was an error with opening a communication");
                }
            } else
            {
                serialPort->close();
                ui->pushButtonConnect->setText("Connect");
                ui->pushButtonSearch->setEnabled(true);
                ui->comboBoxDevicesList->setEnabled(true);
                ui->checkBoxSaveData->setEnabled(true);
                labelIcon->setStyleSheet("background-color: red");
                labelStatus->setText("No connection");
            }
    });

}

MainWindow::~MainWindow()
{
    if(ui->textBrowserLog->toPlainText().length() > 0 && ui->checkBoxSaveData->isChecked())
    {
        QString src = QApplication::applicationDirPath() + "/";

        QDate currentDate = QDate::currentDate();
        QDateTime currentTime = QDateTime::currentDateTime();

        src.append(currentDate.toString("yyyy-MM-dd-"));
        src.append(currentTime.toString("HH-mm-ss"));
        src.append(".txt");

        QFile file(src);

        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        out<<ui->textBrowserLog->toPlainText();

    }
    delete ui;
}

void MainWindow::addLog(QString msg)
{
    QString log = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->textBrowserLog->append(log + "\t" + msg);
}

void MainWindow::readDataFromPort()
{
    while (serialPort->canReadLine()) {
        QString line = serialPort->readLine().trimmed();
        QStringList list = line.split("\t");

        if(list.length() == 9)
        {
            imuCalc->addDataGyro(list.at(0), list.at(1), list.at(2));
            imuCalc->addDataAcc(list.at(3), list.at(4), list.at(5));
            imuCalc->addDataMag(list.at(6), list.at(7), list.at(8));
            addLog(line);
        }
    }
}
