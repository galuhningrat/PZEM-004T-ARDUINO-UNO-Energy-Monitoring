#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
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
    void updateValues();

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    QTimer *timer;

    float voltage;
    float current;
    float power;
    float energy;
    float frequency;
    float powerFactor;
};

#endif // MAINWINDOW_H
