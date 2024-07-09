# Battery Monitoring System dengan Qt dan Arduino (updates & upgrades)

Project ini adalah real-time battery monitoring system menggunakan Qt untuk user interface dan Arduino untuk pengumpulan data. Sistem ini menampilkan berbagai parameter baterai, termasuk tegangan, arus, daya, energi, frekuensi, dan faktor daya, serta menampilkan persentase daya baterai secara visual menggunakan QProgressBar.

## Fitur

- Menampilkan parameter baterai secara real-time
- Visualisasi persentase daya baterai menggunakan QProgressBar

## Kode Program

Dalam kasus yang berbeda, perhitungan persentase ini mengasumsikan bahwa daya yang diterima dari PZEM-004T adalah daya baterai. Dan mungkin perlu menyesuaikan logika perhitungan persentase sesuai dengan kebutuhan project.
Untuk menambahkan QProgressBar yang menampilkan persentase daya baterai secara real-time dari data yang diterima dari Arduino, perlu melakukan beberapa perubahan pada kode Qt. Berikut adalah langkah-langkah dan perubahan yang diperlukan:

1. Tambahkan variabel anggota baru di `mainwindow.h`:

```cpp
private:
    // ... (kode lainnya)
    QProgressBar *batteryProgressBar;
    float maxPower;  // Daya maksimum baterai
```

2. Inisialisasi `batteryProgressBar` dan `maxPower` di konstruktor `MainWindow` dalam `mainwindow.cpp`:

```cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial(new QSerialPort(this))
    , timer(new QTimer(this))
    , maxPower(100.0)  // Sesuaikan dengan daya maksimum baterai Anda
{
    ui->setupUi(this);

    // ... (kode lainnya)

    batteryProgressBar = ui->persentase;
    batteryProgressBar->setRange(0, 100);
    batteryProgressBar->setValue(0);
}
```

3. Modifikasi fungsi `readData()` di `mainwindow.cpp` untuk memperbarui `batteryProgressBar`:

```cpp
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

        // Update battery percentage
        float power = values[2].toFloat();
        int percentage = qRound((power / maxPower) * 100);
        batteryProgressBar->setValue(percentage);
        batteryProgressBar->setFormat(QString("%1%").arg(percentage));

        QString historicalEntry = timeStamp + "," + values.join(',');
        historicalData.append(historicalEntry);
    }
}
```

4. (Kustomisasi) Mengubah tampilan QProgressBar, menambahkan stylesheet di konstruktor `MainWindow`:

```cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial(new QSerialPort(this))
    , timer(new QTimer(this))
    , maxPower(100.0)
{
    ui->setupUi(this);

    // ... (kode lainnya)

    batteryProgressBar = ui->persentase;
    batteryProgressBar->setRange(0, 100);
    batteryProgressBar->setValue(0);
    batteryProgressBar->setTextVisible(true);
    batteryProgressBar->setStyleSheet(
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
}
```

Dengan perubahan ini, QProgressBar akan menampilkan persentase daya baterai secara real-time berdasarkan data yang diterima dari Arduino. Persentase dihitung dengan membandingkan daya saat ini dengan daya maksimum yang telah ditentukan (`maxPower`).

## Catatan Penting

Pastikan untuk menyesuaikan nilai `maxPower` dengan kapasitas maksimum baterai yang Anda gunakan. Nilai ini digunakan untuk menghitung persentase daya baterai.

## Kontribusi

Kontribusi untuk proyek ini sangat diterima. Silakan fork repositori ini dan buat pull request dengan perubahan Anda.

## Visit my Blog
https://mf234.blogspot.com/2024/06/implementasi-pemantauan-data-energi.html
