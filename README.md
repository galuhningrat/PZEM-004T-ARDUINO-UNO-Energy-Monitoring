# Battery Monitoring System dengan Qt dan Arduino

Proyek ini adalah sistem pemantauan baterai real-time menggunakan Qt untuk antarmuka pengguna dan Arduino untuk pengumpulan data. Sistem ini menampilkan berbagai parameter baterai, termasuk tegangan, arus, daya, energi, frekuensi, dan faktor daya, serta menampilkan persentase daya baterai secara visual menggunakan QProgressBar.

## Fitur

- Membaca data dari sensor PZEM-004T melalui Arduino
- Menampilkan parameter baterai secara real-time
- Visualisasi persentase daya baterai menggunakan QProgressBar
- Penyimpanan data historis
- Antarmuka pengguna yang intuitif dengan Qt

## Instalasi

1. Clone repositori ini:
   ```
   git clone https://github.com/username/battery-monitoring-system.git
   ```
2. Buka proyek Qt di Qt Creator
3. Build dan jalankan proyek

## Penggunaan

1. Hubungkan Arduino Anda yang sudah diprogram dengan sensor PZEM-004T ke komputer
2. Jalankan aplikasi Qt
3. Pilih port serial yang sesuai dan klik "Connect"
4. Data baterai akan mulai ditampilkan secara real-time

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

4. Pastikan bahwa QProgressBar dengan nama "persentase" sudah ada di file `mainwindow.ui`. Jika belum, tambahkan melalui Qt Designer atau langsung di kode XML.

5. (Kustomisasi) Jika Anda ingin mengubah tampilan QProgressBar, Anda bisa menambahkan stylesheet di konstruktor `MainWindow`:

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


https://mf234.blogspot.com/2024/06/implementasi-pemantauan-data-energi.html
