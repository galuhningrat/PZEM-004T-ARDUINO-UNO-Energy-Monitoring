#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set up QSerialPort
    arduino = new QSerialPort;
    arduino_port_name = "COM7";
    arduino_is_available = true;

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
        static const char commands[] = {'v', 'c', 'p', 'e', 'f', 'q'};
        static int commandIndex = 0;

        arduino->write(&commands[commandIndex], 1);
        arduino->waitForBytesWritten();

        commandIndex = (commandIndex + 1) % (sizeof(commands) / sizeof(commands[0]));
    } else {
        qDebug() << "Serial port not open.";
    }
}

void MainWindow::readSerial()
{
    while (arduino->canReadLine()) {
        QByteArray serialData = arduino->readLine();
        QString data = QString::fromStdString(serialData.toStdString()).trimmed();
        bool ok;
        float value = data.toFloat(&ok);
        static char lastCommand = '\0';

        if (ok) {
            switch (lastCommand) {
            case 'v':
                voltage = value;
                break;
            case 'c':
                current = value;
                break;
            case 'p':
                power = value;
                break;
            case 'e':
                energy = value;
                break;
            case 'f':
                frequency = static_cast<int>(value);
                break;
            case 'q':
                powerFactor = value;
                break;
            }
            updateValues();
        }

        if (!data.isEmpty()) {
            lastCommand = data[0].toLatin1();
        }
    }
}

void MainWindow::updateValues()
{
    ui->timeLabel->setText(QString("Time: %1").arg(QTime::currentTime().toString()));
    ui->voltageLabel->setText(QString("Voltage (V): %1").arg(voltage));
    ui->currentLabel->setText(QString("Current (A): %1").arg(current));
    ui->powerLabel->setText(QString("Power (W): %1").arg(power));
    ui->energyLabel->setText(QString("Energy (kWh): %1").arg(energy));
    ui->frequencyLabel->setText(QString("Frequency (Hz): %1").arg(frequency));
    ui->pfLabel->setText(QString("Power Factor: %1").arg(powerFactor));
}
