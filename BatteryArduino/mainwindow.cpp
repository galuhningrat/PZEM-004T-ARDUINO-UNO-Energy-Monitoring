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

    serial->setPortName("COM10");  // Ganti sesuai port yang digunakan
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

    QList<QLabel*> timeLabels = {ui->historicalTimeLabel1, ui->historicalTimeLabel2, ui->historicalTimeLabel3, ui->historicalTimeLabel4, ui->historicalTimeLabel5, ui->historicalTimeLabel6, ui->historicalTimeLabel7, ui->historicalTimeLabel8, ui->historicalTimeLabel9, ui->historicalTimeLabel10};
    QList<QLabel*> voltageLabels = {ui->historicalVoltageLabel1, ui->historicalVoltageLabel2, ui->historicalVoltageLabel3, ui->historicalVoltageLabel4, ui->historicalVoltageLabel5, ui->historicalVoltageLabel6, ui->historicalVoltageLabel7, ui->historicalVoltageLabel8, ui->historicalVoltageLabel9, ui->historicalVoltageLabel10};
    QList<QLabel*> currentLabels = {ui->historicalCurrentLabel1, ui->historicalCurrentLabel2, ui->historicalCurrentLabel3, ui->historicalCurrentLabel4, ui->historicalCurrentLabel5, ui->historicalCurrentLabel6, ui->historicalCurrentLabel7, ui->historicalCurrentLabel8, ui->historicalCurrentLabel9, ui->historicalCurrentLabel10};
    QList<QLabel*> powerLabels = {ui->historicalPowerLabel1, ui->historicalPowerLabel2, ui->historicalPowerLabel3, ui->historicalPowerLabel4, ui->historicalPowerLabel5, ui->historicalPowerLabel6, ui->historicalPowerLabel7, ui->historicalPowerLabel8, ui->historicalPowerLabel9, ui->historicalPowerLabel10};
    QList<QLabel*> energyLabels = {ui->historicalEnergyLabel1, ui->historicalEnergyLabel2, ui->historicalEnergyLabel3, ui->historicalEnergyLabel4, ui->historicalEnergyLabel5, ui->historicalEnergyLabel6, ui->historicalEnergyLabel7, ui->historicalEnergyLabel8, ui->historicalEnergyLabel9, ui->historicalEnergyLabel10};
    QList<QLabel*> frequencyLabels = {ui->historicalFrequencyLabel1, ui->historicalFrequencyLabel2, ui->historicalFrequencyLabel3, ui->historicalFrequencyLabel4, ui->historicalFrequencyLabel5, ui->historicalFrequencyLabel6, ui->historicalFrequencyLabel7, ui->historicalFrequencyLabel8, ui->historicalFrequencyLabel9, ui->historicalFrequencyLabel10};
    QList<QLabel*> pfLabels = {ui->historicalPfLabel1, ui->historicalPfLabel2, ui->historicalPfLabel3, ui->historicalPfLabel4, ui->historicalPfLabel5, ui->historicalPfLabel6, ui->historicalPfLabel7, ui->historicalPfLabel8, ui->historicalPfLabel9, ui->historicalPfLabel10};

    if (values.size() == 7) {
        // Shift existing data
        for (int i = 9; i > 0; --i) {
            timeLabels[i]->setText(timeLabels[i - 1]->text());
            voltageLabels[i]->setText(voltageLabels[i - 1]->text());
            currentLabels[i]->setText(currentLabels[i - 1]->text());
            powerLabels[i]->setText(powerLabels[i - 1]->text());
            energyLabels[i]->setText(energyLabels[i - 1]->text());
            frequencyLabels[i]->setText(frequencyLabels[i - 1]->text());
            pfLabels[i]->setText(pfLabels[i - 1]->text());
        }

        // Update the first row with new data
        timeLabels[0]->setText("Time: " + values[0]);
        voltageLabels[0]->setText("Voltage (V): " + values[1]);
        currentLabels[0]->setText("Current (A): " + values[2]);
        powerLabels[0]->setText("Power (W): " + values[3]);
        energyLabels[0]->setText("Energy (kWh): " + values[4]);
        frequencyLabels[0]->setText("Frequency (Hz): " + values[5]);
        pfLabels[0]->setText("PF: " + values[6]);
    }
}
