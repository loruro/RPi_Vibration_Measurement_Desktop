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
    Ui::MainWindow *ui;
    QLineSeries *series;
    QChart *chart;
    QModbusRtuSerialMaster *modbusDevice;
    QModbusDataUnit *dataUnit;
    QTimer *timer;
    int counter = 0;

private slots:
    void readModbus();
    void readReady();
};

#endif // MAINWINDOW_H
