#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    seriesX = new QLineSeries(this);
    seriesY = new QLineSeries(this);
    seriesZ = new QLineSeries(this);
    seriesX->setName("X axis");
    seriesY->setName("Y axis");
    seriesZ->setName("Z axis");

    chart = new QChart();
    chart->addSeries(seriesX);
    chart->addSeries(seriesY);
    chart->addSeries(seriesZ);
    chart->createDefaultAxes();
    chart->axisX()->setRange(0,30);
    chart->axisY()->setRange(-2, 2);
    chart->axisX()->setTitleText("Time [s]");
    chart->axisY()->setTitleText("Acceleration [g]");
    chart->setTitle("Data");

    ui->chart->setChart(chart);
    ui->chart->setRenderHint(QPainter::Antialiasing);

    modbusManager = new ModbusManager();
    modbusManager->moveToThread(&modbusThread);
    connect(modbusManager, &ModbusManager::updateChart, this, &MainWindow::updateChart);
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

void MainWindow::updateChart(QList<QPointF> dataX, QList<QPointF> dataY, QList<QPointF> dataZ, qreal counter)
{
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

void MainWindow::updateStatusBar(quint16 failures)
{
    statusBar()->showMessage(QString("Failures: %1").arg(QString::number(failures)));
}
