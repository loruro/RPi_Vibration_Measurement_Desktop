#ifndef MODBUSMANAGER_H
#define MODBUSMANAGER_H

#include <QObject>
#include <QModbusRtuSerialMaster>
#include <QTimer>
#include <QPointF>

class ModbusManager : public QObject
{
    Q_OBJECT
public:
    explicit ModbusManager(QObject *parent = nullptr);

public slots:
    void start();

private:
    const int bufferLength = 20;

    QModbusRtuSerialMaster *modbusDevice;
    QModbusDataUnit *dataUnit;
    QModbusDataUnit *dataProcessedUnit;
    QModbusDataUnit *temperatureUnit;
    QModbusDataUnit *failureUnit; // Test
    QTimer *timer;
    QTimer *processedTimer;
    QTimer *temperatureTimer;
    QTimer *failureTimer; // Test
    qreal counter = 0;
    qreal counterProcessed = 0;
    qreal counterTemperature = 0;

signals:
    void updateChart(QList<QPointF> dataX, QList<QPointF> dataY, QList<QPointF> dataZ);
    void updateProcessedSeries(QList<QPointF> samples);
    void updateTemperatureSeries(qreal x, qreal y);
    void updateStatusBar(quint16 fifoOverrunRpi, quint16 fifoOverrunAdxl, quint16 fifoOverrunProcessed); // Test

private slots:
    void readData();
    void readDataReady();
    void readProcessedData();
    void readProcessedDataReady();
    void readTemperature();
    void readTemperatureReady();
    void readFailure(); // Test
    void readFailureReady(); // Test
};

#endif // MODBUSMANAGER_H
