#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include <QLabel>
#include <QProgressBar>

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

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QTimer *timer;
    QStringList historicalData;

    QProgressBar *powerProgressBar;
    float maxExpectedPower;

    QLabel *currentTimeValue;
    QLabel *currentVoltageValue;
    QLabel *currentCurrentValue;
    QLabel *currentPowerValue;
    QLabel *currentEnergyValue;
    QLabel *currentFrequencyValue;
    QLabel *currentPfValue;

    QList<QLabel*> historicalTimeLabels;
    QList<QLabel*> historicalVoltageLabels;
    QList<QLabel*> historicalCurrentLabels;
    QList<QLabel*> historicalPowerLabels;
    QList<QLabel*> historicalEnergyLabels;
    QList<QLabel*> historicalFrequencyLabels;
    QList<QLabel*> historicalPfLabels;

    void updateHistoricalData(QString data);
};

#endif // MAINWINDOW_H
