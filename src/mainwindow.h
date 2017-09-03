#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtCharts/QLineSeries>
#include <QModbusRtuSerialMaster>

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
    int bufferLength = 20;

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

private slots:
    void readReadyBit();
    void readReadyBitReady();
    void readData();
    void readDataReady();
};

#endif // MAINWINDOW_H
