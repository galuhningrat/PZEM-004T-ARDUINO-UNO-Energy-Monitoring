#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QSerialPort>
#include <QTimer>
#include <QDateTime> // Tambahkan ini

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void readSerialData();
    void updateTime();

private:
    QLabel *timeLabel;
    QLabel *voltageLabel;
    QLabel *currentLabel;
    QLabel *powerLabel;
    QLabel *energyLabel;
    QLabel *frequencyLabel;
    QLabel *pfLabel;
    QSerialPort *serial;
    QTimer *timer;
};

#endif // MAINWINDOW_H
