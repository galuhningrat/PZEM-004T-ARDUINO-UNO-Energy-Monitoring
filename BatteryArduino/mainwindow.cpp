#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial(new QSerialPort(this))
    , timer(new QTimer(this))
    , maxExpectedPower(6.0)
{
    ui->setupUi(this);

    // Connect the labels to the UI
    ui->verticalLayoutTime->addWidget(ui->currentTimeValue);
    ui->verticalLayoutVoltage->addWidget(ui->currentVoltageValue);
    ui->verticalLayoutCurrent->addWidget(ui->currentCurrentValue);
    ui->verticalLayoutPower->addWidget(ui->currentPowerValue);
    ui->verticalLayoutEnergy->addWidget(ui->currentEnergyValue);
    ui->verticalLayoutFrequency->addWidget(ui->currentFrequencyValue);
    ui->verticalLayoutPf->addWidget(ui->currentPfValue);

    // Initialize the historical data label lists
    historicalTimeLabels = {
        ui->historicalTimeLabel1, ui->historicalTimeLabel2, ui->historicalTimeLabel3,
        ui->historicalTimeLabel4, ui->historicalTimeLabel5, ui->historicalTimeLabel6,
        ui->historicalTimeLabel7, ui->historicalTimeLabel8, ui->historicalTimeLabel9,
        ui->historicalTimeLabel10
    };

    historicalVoltageLabels = {
        ui->historicalVoltageLabel1, ui->historicalVoltageLabel2, ui->historicalVoltageLabel3,
        ui->historicalVoltageLabel4, ui->historicalVoltageLabel5, ui->historicalVoltageLabel6,
        ui->historicalVoltageLabel7, ui->historicalVoltageLabel8, ui->historicalVoltageLabel9,
        ui->historicalVoltageLabel10
    };

    historicalCurrentLabels = {
        ui->historicalCurrentLabel1, ui->historicalCurrentLabel2, ui->historicalCurrentLabel3,
        ui->historicalCurrentLabel4, ui->historicalCurrentLabel5, ui->historicalCurrentLabel6,
        ui->historicalCurrentLabel7, ui->historicalCurrentLabel8, ui->historicalCurrentLabel9,
        ui->historicalCurrentLabel10
    };

    historicalPowerLabels = {
        ui->historicalPowerLabel1, ui->historicalPowerLabel2, ui->historicalPowerLabel3,
        ui->historicalPowerLabel4, ui->historicalPowerLabel5, ui->historicalPowerLabel6,
        ui->historicalPowerLabel7, ui->historicalPowerLabel8, ui->historicalPowerLabel9,
        ui->historicalPowerLabel10
    };

    historicalEnergyLabels = {
        ui->historicalEnergyLabel1, ui->historicalEnergyLabel2, ui->historicalEnergyLabel3,
        ui->historicalEnergyLabel4, ui->historicalEnergyLabel5, ui->historicalEnergyLabel6,
        ui->historicalEnergyLabel7, ui->historicalEnergyLabel8, ui->historicalEnergyLabel9,
        ui->historicalEnergyLabel10
    };

    historicalFrequencyLabels = {
        ui->historicalFrequencyLabel1, ui->historicalFrequencyLabel2, ui->historicalFrequencyLabel3,
        ui->historicalFrequencyLabel4, ui->historicalFrequencyLabel5, ui->historicalFrequencyLabel6,
        ui->historicalFrequencyLabel7, ui->historicalFrequencyLabel8, ui->historicalFrequencyLabel9,
        ui->historicalFrequencyLabel10
    };

    historicalPfLabels = {
        ui->historicalPfLabel1, ui->historicalPfLabel2, ui->historicalPfLabel3,
        ui->historicalPfLabel4, ui->historicalPfLabel5, ui->historicalPfLabel6,
        ui->historicalPfLabel7, ui->historicalPfLabel8, ui->historicalPfLabel9,
        ui->historicalPfLabel10
    };

    powerProgressBar = ui->persentase;
    powerProgressBar->setRange(0, 100);
    powerProgressBar->setValue(0);
    powerProgressBar->setTextVisible(true);
    powerProgressBar->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid grey;"
        "   border-radius: 5px;"
        "   text-align: center;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #05B8CC;"
        "   width: 20px;"
        "}"
        );

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
    timer->start(1000);  // Update historical data every 2 seconds
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

        ui->currentTimeValue->setText(timeStamp);
        ui->currentVoltageValue->setText(values[0]);
        ui->currentCurrentValue->setText(values[1]);
        ui->currentPowerValue->setText(values[2]);
        ui->currentEnergyValue->setText(values[3]);
        ui->currentFrequencyValue->setText(values[4]);
        ui->currentPfValue->setText(values[5]);

        float power = values[2].toFloat();
        int percentage = qRound((power / maxExpectedPower) * 100);
        percentage = qMin(percentage, 100);
        powerProgressBar->setValue(percentage);
        powerProgressBar->setFormat(QString("%1% (%2W / %3W)").arg(percentage).arg(power, 0, 'f', 1).arg(maxExpectedPower, 0, 'f', 1));

        QString historicalEntry = timeStamp + "," + values.join(',');
        historicalData.append(historicalEntry);
    }
}

void MainWindow::updateHistoricalData(QString data)
{
    QStringList values = data.split(',');

    if (values.size() == 7) {
        // Update the historical data labels
        historicalTimeLabels[0]->setText(values[0]);
        historicalVoltageLabels[0]->setText(values[1]);
        historicalCurrentLabels[0]->setText(values[2]);
        historicalPowerLabels[0]->setText(values[3]);
        historicalEnergyLabels[0]->setText(values[4]);
        historicalFrequencyLabels[0]->setText(values[5]);
        historicalPfLabels[0]->setText(values[6]);

        // Shift the remaining data down
        for (int i = 9; i >= 1; i--) {
            historicalTimeLabels[i]->setText(historicalTimeLabels[i - 1]->text());
            historicalVoltageLabels[i]->setText(historicalVoltageLabels[i - 1]->text());
            historicalCurrentLabels[i]->setText(historicalCurrentLabels[i - 1]->text());
            historicalPowerLabels[i]->setText(historicalPowerLabels[i - 1]->text());
            historicalEnergyLabels[i]->setText(historicalEnergyLabels[i - 1]->text());
            historicalFrequencyLabels[i]->setText(historicalFrequencyLabels[i - 1]->text());
            historicalPfLabels[i]->setText(historicalPfLabels[i - 1]->text());
        }
    }
}
