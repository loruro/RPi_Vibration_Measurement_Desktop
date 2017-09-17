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
    QChart *chart;

    QLineSeries *seriesX;
    QLineSeries *seriesY;
    QLineSeries *seriesZ;
    QLineSeries *seriesTemperature;
    QValueAxis *temperatureAxis;

    QLineSeries *seriesRMSX;
    QLineSeries *seriesRMSY;
    QLineSeries *seriesRMSZ;
    QLineSeries *seriesVRMSX;
    QLineSeries *seriesVRMSY;
    QLineSeries *seriesVRMSZ;
    QLineSeries *seriesPPX;
    QLineSeries *seriesPPY;
    QLineSeries *seriesPPZ;
    QLineSeries *seriesKurtX;
    QLineSeries *seriesKurtY;
    QLineSeries *seriesKurtZ;
    QValueAxis *axisX;
    QValueAxis *axisRMS;
    QValueAxis *axisVRMS;
    QValueAxis *axisPP;
    QValueAxis *axisKurt;

    void toggleGroupBox(QGroupBox* group, bool enable);

private slots:
    void on_radio_live_raw_toggled(bool checked);
    void on_radio_live_proc_toggled(bool checked);
    void on_radio_record_raw_toggled(bool checked);
    void on_radio_record_proc_toggled(bool checked);
    void on_button_start_clicked();
    void on_button_stop_clicked();
    void updateChart(QList<QPointF> dataX, QList<QPointF> dataY, QList<QPointF> dataZ);
    void updateProcessedSeries(QList<QPointF> samples);
    void updateTemperatureSeries(qreal x, qreal y);
    void updateStatusBar(quint16 fifoOverrunRpi, quint16 fifoOverrunAdxl, quint16 fifoOverrunProcessed); // Test
};

#endif // MAINWINDOW_H
