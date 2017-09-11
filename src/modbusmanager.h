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
    QModbusDataUnit *failureUnit; // Test
    QTimer *timer;
    QTimer *failureTimer; // Test
    qreal counter = 0;

signals:
    void updateChart(QList<QPointF> dataX, QList<QPointF> dataY, QList<QPointF> dataZ, qreal counter);
    void updateStatusBar(quint16 fifoOverrunRpi, quint16 fifoOverrunAdxl); // Test

private slots:
    void readData();
    void readDataReady();
    void readFailure(); // Test
    void readFailureReady(); // Test
};

#endif // MODBUSMANAGER_H
