#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
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
    chart->axisY()->setRange(-2, 2);
    chart->axisX()->setTitleText("Time [s]");
    chart->axisY()->setTitleText("Acceleration [g]");
    chart->setTitle("Data");

    ui->chart->setChart(chart);
    ui->chart->setRenderHint(QPainter::Antialiasing);

    modbusDevice = new QModbusRtuSerialMaster(this);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, "COM4");
    modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud115200);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
    modbusDevice->setTimeout(250);
    if (!modbusDevice->connectDevice()) {
        qDebug("Connect failed\n");
    }

    dataUnit = new QModbusDataUnit(QModbusDataUnit::InputRegisters, 0, 6 * bufferLength);
    dataReadyUnit = new QModbusDataUnit(QModbusDataUnit::DiscreteInputs, 0, 1);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readReadyBit()));
    timer->start(200);
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

void MainWindow::readReadyBit()
{
    if (auto *reply = modbusDevice->sendReadRequest(*dataReadyUnit, 0x0A)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MainWindow::readReadyBitReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        qDebug("Read error\n");
    }
}

void MainWindow::readReadyBitReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        if (unit.value(0)) {
            readData();
        }
    } else {
        qDebug("Read ready bit response error: %d\n", reply->error());
    }

    reply->deleteLater();
}

void MainWindow::readData()
{
    if (auto *reply = modbusDevice->sendReadRequest(*dataUnit, 0x0A)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MainWindow::readDataReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        qDebug("Read error\n");
    }
}

void MainWindow::readDataReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        for (int i = 0; i < bufferLength; i++) {
            float data[3];
            for (int j = 0; j < 6; j++) {
                *((uint16_t *)data + j) = unit.value(i * 6 + j);
            }
            seriesX->append(counter, data[0]);
            seriesY->append(counter, data[1]);
            seriesZ->append(counter, data[2]);
            counter+=0.01;
        }
        chart->axisX()->setRange(0,counter+1);

    } else {
        qDebug("Read data response error: %d\n", reply->error());
    }

    reply->deleteLater();
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
