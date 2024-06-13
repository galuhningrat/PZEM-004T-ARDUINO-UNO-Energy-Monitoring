#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <QtGui>
#include <QMainWindow>
#include <QTimer>

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
    void readData();
    void readSerial();
    void updateValues();

private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    QTimer *timer;
    QString arduino_port_name;
    bool arduino_is_available;

    float voltage;
    float current;
    float power;
    float energy;
    int frequency;
    float powerFactor;
};

#endif // MAINWINDOW_H
