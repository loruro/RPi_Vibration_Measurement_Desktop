#include "modbusmanager.h"

#include <QSerialPort>
#include <QList>
#include <QVariant>

ModbusManager::ModbusManager(QObject *parent) : QObject(parent)
{
    dataUnit = new QModbusDataUnit(QModbusDataUnit::InputRegisters, 2, 6 * bufferLength);
    failureUnit = new QModbusDataUnit(QModbusDataUnit::InputRegisters, 999, 1); // Test

    timer = new QTimer(this);
    failureTimer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readData()));
    connect(failureTimer, SIGNAL(timeout()), this, SLOT(readFailure()));
}

void ModbusManager::start()
{
    modbusDevice = new QModbusRtuSerialMaster(this);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, "COM4");
    modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud115200);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
    modbusDevice->setTimeout(500);
    if (!modbusDevice->connectDevice()) {
        qDebug("Connect failed\n");
    }

    timer->start(150);
    failureTimer->start(5000);
}

void ModbusManager::readData()
{
    if (auto *reply = modbusDevice->sendReadRequest(*dataUnit, 0x0A)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &ModbusManager::readDataReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        qDebug("Read error\n");
    }
}

void ModbusManager::readDataReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        QList<QPointF> datapointListX;
        QList<QPointF> datapointListY;
        QList<QPointF> datapointListZ;
        const QModbusDataUnit unit = reply->result();
        for (int i = 0; i < bufferLength; i++) {
            float dataX, dataY, dataZ;
            *((uint16_t *)&dataX + 0) = unit.value(i * 2 + 0 + 0);
            *((uint16_t *)&dataX + 1) = unit.value(i * 2 + 0 + 1);
            *((uint16_t *)&dataY + 0) = unit.value(i * 2 + 40 + 0);
            *((uint16_t *)&dataY + 1) = unit.value(i * 2 + 40 + 1);
            *((uint16_t *)&dataZ + 0) = unit.value(i * 2 + 80 + 0);
            *((uint16_t *)&dataZ + 1) = unit.value(i * 2 + 80 + 1);
            datapointListX.append(QPointF(counter, dataX));
            datapointListY.append(QPointF(counter, dataY));
            datapointListZ.append(QPointF(counter, dataZ));
            counter+=0.01;
        }
        emit updateChart(datapointListX, datapointListY, datapointListZ, counter);

    } else {
        qDebug("Read data response error: %d\n", reply->error());
    }

    reply->deleteLater();
}

void ModbusManager::readFailure()
{
    if (auto *reply = modbusDevice->sendReadRequest(*failureUnit, 0x0A)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &ModbusManager::readFailureReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        qDebug("Read error\n");
    }
}

void ModbusManager::readFailureReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        emit updateStatusBar(unit.value(0));
    } else {
        qDebug("Read failure response error: %d\n", reply->error());
    }

    reply->deleteLater();
}
