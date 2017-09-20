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

signals:
    void start(int mode, int rawOrVel, double step, int recordTime);
    void stop();

private:
    const qreal xSeriesLength = 30.0;

    Ui::MainWindow *ui;
    QThread modbusThread;
    ModbusManager *modbusManager;
    QChart *chart;

    QLineSeries *seriesX;
    QLineSeries *seriesY;
    QLineSeries *seriesZ;
    QLineSeries *seriesVelocityX;
    QLineSeries *seriesVelocityY;
    QLineSeries *seriesVelocityZ;
    QLineSeries *seriesTemperature;
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
    QValueAxis *axisRaw;
    QValueAxis *axisVelocity;
    QValueAxis *temperatureAxis;
    QValueAxis *axisRMS;
    QValueAxis *axisVRMS;
    QValueAxis *axisPP;
    QValueAxis *axisKurt;

    int mode = 0;

    void toggleGroupBox(QGroupBox* group, bool enable);
    void clearSeries();
    void clearChart();
    void setChartA();
    void setChartB();

private slots:
    void on_radio_live_raw_toggled(bool checked);
    void on_radio_live_proc_toggled(bool checked);
    void on_radio_record_raw_toggled(bool checked);
    void on_radio_record_proc_toggled(bool checked);
    void on_button_start_clicked();
    void on_button_stop_clicked();
    void updateChart(QList<QPointF> dataX, QList<QPointF> dataY, QList<QPointF> dataZ, bool rawOrVel);
    void updateProcessedSeries(QList<QPointF> samples);
    void updateTemperatureSeries(qreal x, qreal y);
    void updateStatusBar(quint16 fifoOverrunAdxl, quint16 fifoOverrunRawA, quint16 fifoOverrunRawB, quint16 fifoOverrunVelocity, quint16 fifoOverrunProcessed); // Test
};

#endif // MAINWINDOW_H
