#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serialPort = new QSerialPort(this);
    serialPort->setPortName("/dev/ttyUSB0"); // Replace with the appropriate serial port name
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->open(QIODevice::ReadWrite);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::readData);
    timer->start(1000); // Read data every 1 second
}

MainWindow::~MainWindow()
{
    delete ui;
    delete serialPort;
    delete timer;
}

void MainWindow::readData()
{
    serialPort->write("v");
    serialPort->waitForBytesWritten();
    if (serialPort->waitForReadyRead()) {
        voltage = serialPort->readLine().toFloat();
    }

    serialPort->write("c");
    serialPort->waitForBytesWritten();
    if (serialPort->waitForReadyRead()) {
        current = serialPort->readLine().toFloat();
    }

    serialPort->write("p");
    serialPort->waitForBytesWritten();
    if (serialPort->waitForReadyRead()) {
        power = serialPort->readLine().toFloat();
    }

    serialPort->write("e");
    serialPort->waitForBytesWritten();
    if (serialPort->waitForReadyRead()) {
        energy = serialPort->readLine().toFloat();
    }

    serialPort->write("f");
    serialPort->waitForBytesWritten();
    if (serialPort->waitForReadyRead()) {
        frequency = serialPort->readLine().toFloat();
    }

    serialPort->write("q");
    serialPort->waitForBytesWritten();
    if (serialPort->waitForReadyRead()) {
        powerFactor = serialPort->readLine().toFloat();
    }

    updateValues();
}

void MainWindow::updateValues()
{
    ui->voltageLabel->setText(QString("Voltage (V): %1").arg(voltage));
    ui->currentLabel->setText(QString("Current (A): %1").arg(current));
    ui->powerLabel->setText(QString("Power (W): %1").arg(power));
    ui->energyLabel->setText(QString("Energy (kWh): %1").arg(energy));
    ui->frequencyLabel->setText(QString("Frequency (Hz): %1").arg(frequency));
    ui->pfLabel->setText(QString("Power Factor: %1").arg(powerFactor));
}
