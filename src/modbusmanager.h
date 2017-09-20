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
    void connectToDevice();
    void start(int _mode, int _rawOrVel, double _step, int _recordTime);
    void stop();

private:
    const int bufferLength = 20;

    QModbusRtuSerialMaster *modbusDevice;
    QModbusDataUnit *dataUnit;
    QModbusDataUnit *dataProcessedUnit;
    QModbusDataUnit *temperatureUnit;
    QModbusDataUnit *failureUnit; // Test
    QModbusDataUnit *setModeUnit;
    QTimer *timer;
    QTimer *processedTimer;
    QTimer *temperatureTimer;
    QTimer *failureTimer; // Test
    qreal counter = 0;
    qreal counterProcessed = 0;
    qreal counterTemperature = 0;

    quint16 frequency;
    quint16 step;
    quint16 mode;
    bool rawOrVel;
    int recordTime;

signals:
    void updateChart(QList<QPointF> dataX, QList<QPointF> dataY, QList<QPointF> dataZ, bool rawOrVel);
    void updateProcessedSeries(QList<QPointF> samples);
    void updateTemperatureSeries(qreal x, qreal y);
    void updateStatusBar(quint16 fifoOverrunAdxl, quint16 fifoOverrunRawA, quint16 fifoOverrunRawB, quint16 fifoOverrunVelocity, quint16 fifoOverrunProcessed); // Test

private slots:
    void readData();
    void readDataReady();
    void readProcessedData();
    void readProcessedDataReady();
    void readTemperature();
    void readTemperatureReady();
    void readFailure(); // Test
    void readFailureReady(); // Test
    void setModeReady();
};

#endif // MODBUSMANAGER_H
