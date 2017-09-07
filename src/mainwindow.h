#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtCharts/QLineSeries>
#include <QModbusRtuSerialMaster>
#include <QGroupBox>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    const int bufferLength = 20;
    const qreal xSeriesLength = 30.0;

    Ui::MainWindow *ui;
    QLineSeries *seriesX;
    QLineSeries *seriesY;
    QLineSeries *seriesZ;
    QChart *chart;
    QModbusRtuSerialMaster *modbusDevice;
    QModbusDataUnit *dataUnit;
    QModbusDataUnit *dataReadyUnit;
    QTimer *timer;
    qreal counter = 0;

    void toggleGroupBox(QGroupBox* group, bool enable);

private slots:
    void readReadyBit();
    void readReadyBitReady();
    void readData();
    void readDataReady();
    void on_radio_live_raw_toggled(bool checked);
    void on_radio_live_proc_toggled(bool checked);
    void on_radio_record_raw_toggled(bool checked);
    void on_radio_record_proc_toggled(bool checked);
    void on_button_start_clicked();
    void on_button_stop_clicked();
};

#endif // MAINWINDOW_H
