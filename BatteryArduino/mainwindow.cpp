#include "mainwindow.h"
#include "ui_mainwindow.h"

#define ARDUINO_UNO_VENDOR_ID 0x2341
#define ARDUINO_UNO_PRODUCT_ID 0x0043

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set up QSerialPort
    arduino = new QSerialPort;
    arduino_is_available = false;
    arduino_port_name = "COM7";

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
            if (serialPortInfo.vendorIdentifier() == ARDUINO_UNO_VENDOR_ID) {
                if (serialPortInfo.productIdentifier() == ARDUINO_UNO_PRODUCT_ID) {
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                    qDebug() << "Arduino found on port:" << arduino_port_name;
                }
            }
        }
    }

    if (arduino_is_available) {
        arduino->setPortName(arduino_port_name);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);

        if (arduino->open(QSerialPort::ReadWrite)) {
            qDebug() << "Serial port opened successfully.";
            // Slot for updating value
            QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
        } else {
            qDebug() << "Failed to open serial port.";
            QMessageBox::warning(this, "Port error", "Couldn't open the serial port.");
        }
    } else {
        QMessageBox::warning(this, "Port error", "Couldn't find Arduino");
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::readData);
    timer->start(1000); // Read data every 1 second
}

MainWindow::~MainWindow()
{
    if (arduino->isOpen()) {
        arduino->close();
    }
    delete ui;
}

void MainWindow::readData()
{
    if (arduino->isOpen()) {
        arduino->write("v");
        arduino->waitForBytesWritten();
        if (arduino->waitForReadyRead()) {
            voltage = arduino->readLine().toFloat();
        }

        arduino->write("c");
        arduino->waitForBytesWritten();
        if (arduino->waitForReadyRead()) {
            current = arduino->readLine().toFloat();
        }

        arduino->write("p");
        arduino->waitForBytesWritten();
        if (arduino->waitForReadyRead()) {
            power = arduino->readLine().toFloat();
        }

        arduino->write("e");
        arduino->waitForBytesWritten();
        if (arduino->waitForReadyRead()) {
            energy = arduino->readLine().toFloat();
        }

        arduino->write("f");
        arduino->waitForBytesWritten();
        if (arduino->waitForReadyRead()) {
            frequency = arduino->readLine().toFloat();
        }

        arduino->write("q");
        arduino->waitForBytesWritten();
        if (arduino->waitForReadyRead()) {
            powerFactor = arduino->readLine().toFloat();
        }

        updateValues();
    } else {
        qDebug() << "Serial port not open.";
    }
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
