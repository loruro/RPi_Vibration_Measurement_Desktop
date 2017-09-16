#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qRegisterMetaType<QList<QPointF>>("QList<QPointF>");
    ui->setupUi(this);
    statusBar()->showMessage(QString("Status: OK"));
    toggleGroupBox(ui->group_live_proc, false);
    toggleGroupBox(ui->group_record_raw, false);
    toggleGroupBox(ui->group_record_proc, false);

    temperatureAxis = new QValueAxis(this);
    temperatureAxis->setTickCount(9);
    temperatureAxis->setRange(-20, 100);
    temperatureAxis->setTitleText("Temperature [°C]");

    seriesX = new QLineSeries(this);
    seriesY = new QLineSeries(this);
    seriesZ = new QLineSeries(this);
    seriesTemperature = new QLineSeries(this);
    seriesX->setUseOpenGL(true);
    seriesY->setUseOpenGL(true);
    seriesZ->setUseOpenGL(true);
    seriesTemperature->setUseOpenGL(true);
    seriesX->setName("X axis");
    seriesY->setName("Y axis");
    seriesZ->setName("Z axis");
    seriesTemperature->setName("Temperature");

    chart = new QChart();
    chart->addSeries(seriesX);
    chart->addSeries(seriesY);
    chart->addSeries(seriesZ);
    chart->createDefaultAxes();
    chart->addSeries(seriesTemperature);
    chart->addAxis(temperatureAxis, Qt::AlignRight);
    chart->axisX()->setRange(0, 30);
    chart->axisY()->setRange(-16, 16);
    static_cast<QValueAxis *>(chart->axisX())->setTickCount(7);
    static_cast<QValueAxis *>(chart->axisY())->setTickCount(9);
    chart->axisX()->setTitleText("Time [s]");
    chart->axisY()->setTitleText("Acceleration [g]");
    chart->setTitle("Data");

    seriesTemperature->attachAxis(chart->axisX());
    seriesTemperature->attachAxis(temperatureAxis);
    seriesTemperature->setColor(Qt::red);

    ui->chart->setChart(chart);
    ui->chart->setRenderHint(QPainter::Antialiasing);

    modbusManager = new ModbusManager();
    modbusManager->moveToThread(&modbusThread);
    connect(modbusManager, &ModbusManager::updateChart, this, &MainWindow::updateChart);
    connect(modbusManager, &ModbusManager::updateTemperatureSeries, this, &MainWindow::updateTemperatureSeries);
    connect(modbusManager, &ModbusManager::updateStatusBar, this, &MainWindow::updateStatusBar);
    connect(&modbusThread, SIGNAL (started()), modbusManager, SLOT (start()));
    modbusThread.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleGroupBox(QGroupBox *group, bool enable)
{
    foreach (QObject *object, group->children()) {
        QWidget *widget = dynamic_cast<QWidget*>(object);
        if (widget) {
            widget->setEnabled(enable);
        }
    }
}

void MainWindow::on_radio_live_raw_toggled(bool checked)
{
    toggleGroupBox(ui->group_live_raw, checked);
}

void MainWindow::on_radio_live_proc_toggled(bool checked)
{
    toggleGroupBox(ui->group_live_proc, checked);
}

void MainWindow::on_radio_record_raw_toggled(bool checked)
{
    toggleGroupBox(ui->group_record_raw, checked);
}

void MainWindow::on_radio_record_proc_toggled(bool checked)
{
    toggleGroupBox(ui->group_record_proc, checked);
}

void MainWindow::on_button_start_clicked()
{
    ui->button_start->setEnabled(false);
    ui->button_stop->setEnabled(true);
}

void MainWindow::on_button_stop_clicked()
{
    ui->button_start->setEnabled(true);
    ui->button_stop->setEnabled(false);
}

void MainWindow::updateChart(QList<QPointF> dataX, QList<QPointF> dataY, QList<QPointF> dataZ)
{
    qreal counter = dataX.last().x();
    seriesX->append(dataX);
    seriesY->append(dataY);
    seriesZ->append(dataZ);
    if (counter > xSeriesLength) {
        chart->axisX()->setRange(counter - xSeriesLength, counter);
        seriesX->removePoints(0, 20);
        seriesY->removePoints(0, 20);
        seriesZ->removePoints(0, 20);
    }
}

void MainWindow::updateTemperatureSeries(qreal x, qreal y)
{
    seriesTemperature->append(x, y);
    if (x > xSeriesLength) {
        seriesTemperature->removePoints(0, 1);
    }
}

void MainWindow::updateStatusBar(quint16 fifoOverrunRpi, quint16 fifoOverrunAdxl)
{
    statusBar()->showMessage(QString("FIFO Overrun. RPi: %1   ADXL: %2").arg(QString::number(fifoOverrunRpi)).arg(QString::number(fifoOverrunAdxl)));
}
