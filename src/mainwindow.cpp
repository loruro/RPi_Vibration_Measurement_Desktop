#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    series = new QLineSeries(this);

    chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->createDefaultAxes();
    chart->axisY()->setRange(-2, 2);
    chart->setTitle("Simple line chart example");

    ui->chart->setChart(chart);
    ui->chart->setRenderHint(QPainter::Antialiasing);

    modbusDevice = new QModbusRtuSerialMaster(this);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, "COM4");
    modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud115200);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
    modbusDevice->setTimeout(1000);
    if (!modbusDevice->connectDevice()) {
        qDebug("Connect failed\n");
    }

    dataUnit = new QModbusDataUnit(QModbusDataUnit::InputRegisters, 0, 6);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readModbus()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readModbus()
{
    if (auto *reply = modbusDevice->sendReadRequest(*dataUnit, 0x0A)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MainWindow::readReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        qDebug("Read error\n");
    }
}

void MainWindow::readReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        float data[3];
        for (int i = 0; i < 6; i++) {
            *((uint16_t *)data + i) = unit.value(i);
        }
        series->append(counter, data[2]);
        chart->axisX()->setRange(0,counter+1);
        counter++;
    } else {
        qDebug("Read response error\n");
    }

    reply->deleteLater();
}
