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

//    temperatureAxis = new QValueAxis(this);
//    temperatureAxis->setTickCount(9);
//    temperatureAxis->setRange(-20, 100);
//    temperatureAxis->setTitleText("Temperature [°C]");

//    seriesX = new QLineSeries(this);
//    seriesY = new QLineSeries(this);
//    seriesZ = new QLineSeries(this);
//    seriesTemperature = new QLineSeries(this);
//    seriesX->setUseOpenGL(true);
//    seriesY->setUseOpenGL(true);
//    seriesZ->setUseOpenGL(true);
//    seriesTemperature->setUseOpenGL(true);
//    seriesX->setName("X axis");
//    seriesY->setName("Y axis");
//    seriesZ->setName("Z axis");
//    seriesTemperature->setName("Temperature");

//    chart = new QChart();
//    chart->addSeries(seriesX);
//    chart->addSeries(seriesY);
//    chart->addSeries(seriesZ);
//    chart->createDefaultAxes();
//    chart->addSeries(seriesTemperature);
//    chart->addAxis(temperatureAxis, Qt::AlignRight);
//    chart->axisX()->setRange(0, 30);
//    chart->axisY()->setRange(-16, 16);
//    static_cast<QValueAxis *>(chart->axisX())->setTickCount(7);
//    static_cast<QValueAxis *>(chart->axisY())->setTickCount(9);
//    chart->axisX()->setTitleText("Time [s]");
//    chart->axisY()->setTitleText("Acceleration [g]");
//    chart->setTitle("Data");

//    seriesTemperature->attachAxis(chart->axisX());
//    seriesTemperature->attachAxis(temperatureAxis);
//    seriesTemperature->setColor(Qt::red);

    axisX = new QValueAxis(this);
    axisX->setTickCount(7);
    axisX->setRange(0, 30);
    axisX->setTitleText("Time [s]");
    axisRMS = new QValueAxis(this);
    axisRMS->setTickCount(9);
    axisRMS->setRange(-16, 16);
    axisRMS->setTitleText("aaa");
    axisVRMS = new QValueAxis(this);
    axisVRMS->setTickCount(9);
    axisVRMS->setRange(-16, 16);
    axisVRMS->setTitleText("aaa");
    axisPP = new QValueAxis(this);
    axisPP->setTickCount(9);
    axisPP->setRange(-16, 16);
    axisPP->setTitleText("aaa");
    axisKurt = new QValueAxis(this);
    axisKurt->setTickCount(9);
    axisKurt->setRange(-16, 16);
    axisKurt->setTitleText("aaa");

    seriesRMSX = new QLineSeries(this);
    seriesRMSY = new QLineSeries(this);
    seriesRMSZ = new QLineSeries(this);
    seriesVRMSX = new QLineSeries(this);
    seriesVRMSY = new QLineSeries(this);
    seriesVRMSZ = new QLineSeries(this);
    seriesPPX = new QLineSeries(this);
    seriesPPY = new QLineSeries(this);
    seriesPPZ = new QLineSeries(this);
    seriesKurtX = new QLineSeries(this);
    seriesKurtY = new QLineSeries(this);
    seriesKurtZ = new QLineSeries(this);
    seriesRMSX->setUseOpenGL(true);
    seriesRMSY->setUseOpenGL(true);
    seriesRMSZ->setUseOpenGL(true);
    seriesVRMSX->setUseOpenGL(true);
    seriesVRMSY->setUseOpenGL(true);
    seriesVRMSZ->setUseOpenGL(true);
    seriesPPX->setUseOpenGL(true);
    seriesPPY->setUseOpenGL(true);
    seriesPPZ->setUseOpenGL(true);
    seriesKurtX->setUseOpenGL(true);
    seriesKurtY->setUseOpenGL(true);
    seriesKurtZ->setUseOpenGL(true);
    seriesRMSX->setName("X axis");
    seriesRMSY->setName("Y axis");
    seriesRMSZ->setName("Z axis");
    seriesVRMSX->setName("X axis");
    seriesVRMSY->setName("Y axis");
    seriesVRMSZ->setName("Z axis");
    seriesPPX->setName("X axis");
    seriesPPY->setName("Y axis");
    seriesPPZ->setName("Z axis");
    seriesKurtX->setName("X axis");
    seriesKurtY->setName("Y axis");
    seriesKurtZ->setName("Z axis");

    chart = new QChart();
    chart->addSeries(seriesRMSX);
    chart->addSeries(seriesRMSY);
    chart->addSeries(seriesRMSZ);
    chart->addSeries(seriesVRMSX);
    chart->addSeries(seriesVRMSY);
    chart->addSeries(seriesVRMSZ);
    chart->addSeries(seriesPPX);
    chart->addSeries(seriesPPY);
    chart->addSeries(seriesPPZ);
    chart->addSeries(seriesKurtX);
    chart->addSeries(seriesKurtY);
    chart->addSeries(seriesKurtZ);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisRMS, Qt::AlignLeft);
    chart->addAxis(axisVRMS, Qt::AlignLeft);
    chart->addAxis(axisPP, Qt::AlignLeft);
    chart->addAxis(axisKurt, Qt::AlignLeft);
    chart->setTitle("Data");

    seriesRMSX->attachAxis(chart->axisX());
    seriesRMSX->attachAxis(axisRMS);
    seriesRMSY->attachAxis(chart->axisX());
    seriesRMSY->attachAxis(axisRMS);
    seriesRMSZ->attachAxis(chart->axisX());
    seriesRMSZ->attachAxis(axisRMS);
    seriesVRMSX->attachAxis(chart->axisX());
    seriesVRMSX->attachAxis(axisVRMS);
    seriesVRMSY->attachAxis(chart->axisX());
    seriesVRMSY->attachAxis(axisVRMS);
    seriesVRMSZ->attachAxis(chart->axisX());
    seriesVRMSZ->attachAxis(axisVRMS);
    seriesPPX->attachAxis(chart->axisX());
    seriesPPX->attachAxis(axisPP);
    seriesPPY->attachAxis(chart->axisX());
    seriesPPY->attachAxis(axisPP);
    seriesPPZ->attachAxis(chart->axisX());
    seriesPPZ->attachAxis(axisPP);
    seriesKurtX->attachAxis(chart->axisX());
    seriesKurtX->attachAxis(axisKurt);
    seriesKurtY->attachAxis(chart->axisX());
    seriesKurtY->attachAxis(axisKurt);
    seriesKurtZ->attachAxis(chart->axisX());
    seriesKurtZ->attachAxis(axisKurt);

    ui->chart->setChart(chart);
    ui->chart->setRenderHint(QPainter::Antialiasing);

    modbusManager = new ModbusManager();
    modbusManager->moveToThread(&modbusThread);
    connect(modbusManager, &ModbusManager::updateChart, this, &MainWindow::updateChart);
    connect(modbusManager, &ModbusManager::updateProcessedSeries, this, &MainWindow::updateProcessedSeries);
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

void MainWindow::updateProcessedSeries(QList<QPointF> samples)
{
    qreal counter = samples.last().x();
    seriesRMSX->append(samples.at(0));
    seriesRMSY->append(samples.at(1));
    seriesRMSZ->append(samples.at(2));
    seriesVRMSX->append(samples.at(3));
    seriesVRMSY->append(samples.at(4));
    seriesVRMSZ->append(samples.at(5));
    seriesPPX->append(samples.at(6));
    seriesPPY->append(samples.at(7));
    seriesPPZ->append(samples.at(8));
    seriesKurtX->append(samples.at(9));
    seriesKurtY->append(samples.at(10));
    seriesKurtZ->append(samples.at(11));
    if (counter > xSeriesLength) {
        chart->axisX()->setRange(counter - xSeriesLength, counter);
        seriesRMSX->removePoints(0, 1);
        seriesRMSY->removePoints(0, 1);
        seriesRMSZ->removePoints(0, 1);
        seriesVRMSX->removePoints(0, 1);
        seriesVRMSY->removePoints(0, 1);
        seriesVRMSZ->removePoints(0, 1);
        seriesPPX->removePoints(0, 1);
        seriesPPY->removePoints(0, 1);
        seriesPPZ->removePoints(0, 1);
        seriesKurtX->removePoints(0, 1);
        seriesKurtY->removePoints(0, 1);
        seriesKurtZ->removePoints(0, 1);
    }
}

void MainWindow::updateTemperatureSeries(qreal x, qreal y)
{
    seriesTemperature->append(x, y);
    if (x > xSeriesLength) {
        seriesTemperature->removePoints(0, 1);
    }
}

void MainWindow::updateStatusBar(quint16 fifoOverrunAdxl, quint16 fifoOverrunRawA, quint16 fifoOverrunRawB, quint16 fifoOverrunVelocity, quint16 fifoOverrunProcessed)
{
    statusBar()->showMessage(QString("FIFO Overrun. ADXL: %1   RawA: %2   RawB: %3   Velocity: %4   Processing: %5").arg(QString::number(fifoOverrunAdxl)).arg(QString::number(fifoOverrunRawA)).arg(QString::number(fifoOverrunRawB)).arg(QString::number(fifoOverrunVelocity)).arg(QString::number(fifoOverrunProcessed)));
}
