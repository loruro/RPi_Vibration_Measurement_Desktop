#include "modbusmanager.h"

#include <QSerialPort>
#include <QList>
#include <QVariant>
#include <QThread>

ModbusManager::ModbusManager(QObject *parent) : QObject(parent)
{
    dataUnit = new QModbusDataUnit(QModbusDataUnit::InputRegisters, 2, 6 * bufferLength);
    dataProcessedUnit = new QModbusDataUnit(QModbusDataUnit::InputRegisters, 242, 24);
    temperatureUnit = new QModbusDataUnit(QModbusDataUnit::InputRegisters, 266, 2);
    failureUnit = new QModbusDataUnit(QModbusDataUnit::InputRegisters, 999, 5); // Test
    setModeUnit = new QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0, 3);

    timer = new QTimer(this);
    processedTimer = new QTimer(this);
    temperatureTimer = new QTimer(this);
    failureTimer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    processedTimer->setTimerType(Qt::PreciseTimer);
    temperatureTimer->setTimerType(Qt::PreciseTimer);
    connect(timer, SIGNAL(timeout()), this, SLOT(readData()));
    connect(processedTimer, SIGNAL(timeout()), this, SLOT(readProcessedData()));
    connect(temperatureTimer, SIGNAL(timeout()), this, SLOT(readTemperature()));
    connect(failureTimer, SIGNAL(timeout()), this, SLOT(readFailure()));
}

void ModbusManager::connectToDevice()
{
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

//    timer->start(200);
//    processedTimer->start(100);
//    temperatureTimer->start(1000);
//    failureTimer->start(5000);
}

void ModbusManager::start(int _mode, int _rawOrVel, double _step, int _recordTime)
{
    counter = 0.0;
    counterProcessed = 0.0;
    counterTemperature = 0.0;
    rawOrVel = _rawOrVel;
    recordTime = _recordTime;

    switch (_mode) {
    case 0:
        if (!rawOrVel) {
            dataUnit->setStartAddress(2);
        } else {
            dataUnit->setStartAddress(122);
        }
        frequency = 100;
        step = 100;
        mode = 0;
        break;
    case 1:
        frequency = 800;
        step = (quint16)(_step * 1000);
        mode = 1;
        break;
    case 2:
        if (!rawOrVel) {
            dataUnit->setStartAddress(2);
        } else {
            dataUnit->setStartAddress(122);
        }
        frequency = 800;
        step = 100;
        mode = 2;
        break;
    case 3:
        frequency = 800;
        step = (quint16)(_step * 1000);
        mode = 3;
        break;
    default:
        break;
    }

    setModeUnit->setValue(0, frequency);
    setModeUnit->setValue(1, step);
    setModeUnit->setValue(2, mode);
    if (auto *reply = modbusDevice->sendWriteRequest(*setModeUnit, 0x0A)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &ModbusManager::setModeReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        qDebug("Read error\n");
    }
}

void ModbusManager::stop()
{
    timer->stop();
    processedTimer->stop();
    temperatureTimer->stop();
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
            counter += 1.0 / frequency;
        }
        emit updateChart(datapointListX, datapointListY, datapointListZ, rawOrVel);

    } else {
        qDebug("Read data response error: %d\n", reply->error());
    }

    reply->deleteLater();
}

void ModbusManager::readProcessedData()
{
    if (auto *reply = modbusDevice->sendReadRequest(*dataProcessedUnit, 0x0A)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &ModbusManager::readProcessedDataReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        qDebug("Read error\n");
    }
}

void ModbusManager::readProcessedDataReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        QList<QPointF> datapointList;
        const QModbusDataUnit unit = reply->result();
        for (int i = 0; i < 12; ++i) {
            float sample;
            *((uint16_t *)&sample + 0) = unit.value(i * 2 + 0);
            *((uint16_t *)&sample + 1) = unit.value(i * 2 + 1);
            datapointList.append(QPointF(counterProcessed, sample));
        }
        emit updateProcessedSeries(datapointList);
        counterProcessed += (step / 100) / 10.0; // Fighting with floats.
    } else {
        qDebug("Read processed data response error: %d\n", reply->error());
    }

    reply->deleteLater();
}

void ModbusManager::readTemperature()
{
    if (auto *reply = modbusDevice->sendReadRequest(*temperatureUnit, 0x0A)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &ModbusManager::readTemperatureReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        qDebug("Read error\n");
    }
}

void ModbusManager::readTemperatureReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        float temperature;
        const QModbusDataUnit unit = reply->result();
        *((uint16_t *)&temperature + 0) = unit.value(0);
        *((uint16_t *)&temperature + 1) = unit.value(1);
        emit updateTemperatureSeries(counterTemperature, temperature);
        counterTemperature += 1;
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
        emit updateStatusBar(unit.value(0), unit.value(1), unit.value(2), unit.value(3), unit.value(4));
    } else {
        qDebug("Read failure response error: %d\n", reply->error());
    }

    reply->deleteLater();
}

void ModbusManager::setModeReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        switch (mode) {
        case 0:
            temperatureTimer->start(1000);
        case 2:
            timer->start(1000.0 / frequency * 20);
            break;
        case 1:
        case 3:
            processedTimer->start(step);
            break;
        }
    } else {
        qDebug("Set mode response error: %d\n", reply->error());
    }

    reply->deleteLater();
}
