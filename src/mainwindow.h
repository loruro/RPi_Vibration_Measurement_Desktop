#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QGroupBox>
#include <QThread>
#include <QValueAxis>

#include "modbusmanager.h"

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
    const qreal xSeriesLength = 30.0;

    Ui::MainWindow *ui;
    QThread modbusThread;
    ModbusManager *modbusManager;
    QLineSeries *seriesX;
    QLineSeries *seriesY;
    QLineSeries *seriesZ;
    QLineSeries *seriesTemperature;
    QChart *chart;
    QValueAxis *temperatureAxis;

    void toggleGroupBox(QGroupBox* group, bool enable);

private slots:
    void on_radio_live_raw_toggled(bool checked);
    void on_radio_live_proc_toggled(bool checked);
    void on_radio_record_raw_toggled(bool checked);
    void on_radio_record_proc_toggled(bool checked);
    void on_button_start_clicked();
    void on_button_stop_clicked();
    void updateChart(QList<QPointF> dataX, QList<QPointF> dataY, QList<QPointF> dataZ);
    void updateTemperatureSeries(qreal x, qreal y);
    void updateStatusBar(quint16 fifoOverrunRpi, quint16 fifoOverrunAdxl); // Test
};

#endif // MAINWINDOW_H
