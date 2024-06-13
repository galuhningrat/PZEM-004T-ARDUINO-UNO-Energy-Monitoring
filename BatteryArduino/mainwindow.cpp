#include "mainwindow.h"
#include <QDateTime>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    timeLabel = new QLabel("Time: ", this);
    voltageLabel = new QLabel("Voltage: ", this);
    currentLabel = new QLabel("Current: ", this);
    powerLabel = new QLabel("Power: ", this);
    energyLabel = new QLabel("Energy: ", this);
    frequencyLabel = new QLabel("Frequency: ", this);
    pfLabel = new QLabel("Power Factor: ", this);

    layout->addWidget(timeLabel);
    layout->addWidget(voltageLabel);
    layout->addWidget(currentLabel);
    layout->addWidget(powerLabel);
    layout->addWidget(energyLabel);
    layout->addWidget(frequencyLabel);
    layout->addWidget(pfLabel);

    serial = new QSerialPort(this);
    serial->setPortName("COM3"); // Ganti dengan port yang sesuai
    serial->setBaudRate(QSerialPort::Baud9600);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
    serial->open(QIODevice::ReadOnly);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start(1000);
}

void MainWindow::readSerialData() {
    while (serial->canReadLine()) {
        QString line = serial->readLine();
        QStringList data = line.split(" ");
        if (data.size() >= 12) {
            voltageLabel->setText("Voltage: " + data[1] + "V");
            currentLabel->setText("Current: " + data[3] + "A");
            powerLabel->setText("Power: " + data[5] + "W");
            energyLabel->setText("Energy: " + data[7] + "kWh");
            frequencyLabel->setText("Frequency: " + data[9] + "Hz");
            pfLabel->setText("Power Factor: " + data[11]);
        }
    }
}

void MainWindow::updateTime() {
    QDateTime currentTime = QDateTime::currentDateTime();
    timeLabel->setText("Time: " + currentTime.toString("hh:mm:ss"));
}
