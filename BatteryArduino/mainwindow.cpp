#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial(new QSerialPort(this))
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    serial->setPortName("COM10");
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);

    if (serial->open(QIODevice::ReadOnly)) {
        qDebug() << "Serial port opened successfully.";
    } else {
        qDebug() << "Failed to open serial port.";
    }

    connect(timer, &QTimer::timeout, [=]() {
        if (!historicalData.isEmpty()) {
            updateHistoricalData(historicalData.takeFirst());
        }
    });
    timer->start(2000);  // Update historical data every 2 seconds
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    QString dataString(data);
    QStringList values = dataString.split(',');

    if (values.size() == 6) {
        QString timeStamp = QTime::currentTime().toString("hh:mm:ss");

        ui->currentTimeLabel->setText("Time: " + timeStamp);
        ui->currentVoltageLabel->setText("Voltage (V): " + values[0]);
        ui->currentCurrentLabel->setText("Current (A): " + values[1]);
        ui->currentPowerLabel->setText("Power (W): " + values[2]);
        ui->currentEnergyLabel->setText("Energy (kWh): " + values[3]);
        ui->currentFrequencyLabel->setText("Frequency (Hz): " + values[4]);
        ui->currentPfLabel->setText("PF: " + values[5]);

        QString historicalEntry = timeStamp + "," + values.join(',');
        historicalData.append(historicalEntry);
    }
}

void MainWindow::updateHistoricalData(QString data)
{
    QStringList values = data.split(',');

    if (values.size() == 7) {
        ui->historicalTimeLabel1->setText("Time 1: " + values[0]);
        ui->historicalVoltageLabel1->setText("Voltage (V): " + values[1]);
        ui->historicalCurrentLabel1->setText("Current: " + values[2]);
        ui->historicalPowerLabel1->setText("Power: " + values[3]);
        ui->historicalEnergyLabel1->setText("Energy: " + values[4]);
        ui->historicalFrequencyLabel1->setText("Frequency: " + values[5]);
        ui->historicalPfLabel1->setText("PF: " + values[6]);

        if (!historicalData.isEmpty()) {
            QString nextData = historicalData.takeFirst();
            values = nextData.split(',');

            ui->historicalTimeLabel2->setText("Time 2: " + values[0]);
            ui->historicalVoltageLabel2->setText("Voltage (V): " + values[1]);
            ui->historicalCurrentLabel2->setText("Current: " + values[2]);
            ui->historicalPowerLabel2->setText("Power: " + values[3]);
            ui->historicalEnergyLabel2->setText("Energy: " + values[4]);
            ui->historicalFrequencyLabel2->setText("Frequency: " + values[5]);
            ui->historicalPfLabel2->setText("PF: " + values[6]);
        }
    }
}
