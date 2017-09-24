#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDateTime>

// TODO: Cleanup!!!

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

    axisX = new QValueAxis(this);
    axisX->setTickCount(7);
    axisX->setRange(0, 30);
    axisX->setTitleText("Time [s]");
    axisRaw = new QValueAxis(this);
    axisRaw->setTickCount(9);
    axisRaw->setRange(-160, 160);
    axisRaw->setTitleText("Acceleration [m/s²]");
    axisVelocity = new QValueAxis(this);
    axisVelocity->setTickCount(9);
    axisVelocity->setRange(-16000, 16000);
    axisVelocity->setTitleText("Velocity [mm/s]");
    temperatureAxis = new QValueAxis(this);
    temperatureAxis->setTickCount(9);
    temperatureAxis->setRange(-20, 100);
    temperatureAxis->setTitleText("Temperature [°C]");
    axisRMS = new QValueAxis(this);
    axisRMS->setTickCount(9);
    axisRMS->setRange(0, 320);
    axisRMS->setTitleText("RMS [m/s²]");
    axisVRMS = new QValueAxis(this);
    axisVRMS->setTickCount(9);
    axisVRMS->setRange(0, 32000);
    axisVRMS->setTitleText("VRMS [mm/s]");
    axisPP = new QValueAxis(this);
    axisPP->setTickCount(9);
    axisPP->setRange(0, 320);
    axisPP->setTitleText("Peak-to-peak [m/s²]");
    axisKurt = new QValueAxis(this);
    axisKurt->setTickCount(9);
    axisKurt->setRange(0, 32);
    axisKurt->setTitleText("Kurtosis");

    seriesX = new QLineSeries(this);
    seriesY = new QLineSeries(this);
    seriesZ = new QLineSeries(this);
    seriesVelocityX = new QLineSeries(this);
    seriesVelocityY = new QLineSeries(this);
    seriesVelocityZ = new QLineSeries(this);
    seriesTemperature = new QLineSeries(this);
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
    seriesX->setUseOpenGL(true);
    seriesY->setUseOpenGL(true);
    seriesZ->setUseOpenGL(true);
    seriesVelocityX->setUseOpenGL(true);
    seriesVelocityY->setUseOpenGL(true);
    seriesVelocityZ->setUseOpenGL(true);
    seriesTemperature->setUseOpenGL(true);
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
    seriesX->setName("Acceleration X axis");
    seriesY->setName("Acceleration Y axis");
    seriesZ->setName("Acceleration Z axis");
    seriesVelocityX->setName("Velocity X axis");
    seriesVelocityY->setName("Velocity Y axis");
    seriesVelocityZ->setName("Velocity Z axis");
    seriesTemperature->setName("Temperature");
    seriesRMSX->setName("RMS X axis");
    seriesRMSY->setName("RMS Y axis");
    seriesRMSZ->setName("RMS Z axis");
    seriesVRMSX->setName("VRMS X axis");
    seriesVRMSY->setName("VRMS Y axis");
    seriesVRMSZ->setName("VRMS Z axis");
    seriesPPX->setName("Peak-to-peak X axis");
    seriesPPY->setName("Peak-to-peak Y axis");
    seriesPPZ->setName("Peak-to-peak Z axis");
    seriesKurtX->setName("Kurtosis X axis");
    seriesKurtY->setName("Kurtosis Y axis");
    seriesKurtZ->setName("Kurtosis Z axis");

    chart = new QChart();
    chart->addSeries(seriesX);
    chart->addSeries(seriesY);
    chart->addSeries(seriesZ);
    chart->addSeries(seriesVelocityX);
    chart->addSeries(seriesVelocityY);
    chart->addSeries(seriesVelocityZ);
    chart->addSeries(seriesTemperature);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisRaw, Qt::AlignLeft);
    chart->addAxis(axisVelocity, Qt::AlignLeft);
    chart->addAxis(temperatureAxis, Qt::AlignRight);
    chart->setTitle("Data");

    seriesX->attachAxis(chart->axisX());
    seriesX->attachAxis(axisRaw);
    seriesY->attachAxis(chart->axisX());
    seriesY->attachAxis(axisRaw);
    seriesZ->attachAxis(chart->axisX());
    seriesZ->attachAxis(axisRaw);
    seriesVelocityX->attachAxis(chart->axisX());
    seriesVelocityX->attachAxis(axisVelocity);
    seriesVelocityY->attachAxis(chart->axisX());
    seriesVelocityY->attachAxis(axisVelocity);
    seriesVelocityZ->attachAxis(chart->axisX());
    seriesVelocityZ->attachAxis(axisVelocity);
    seriesTemperature->attachAxis(chart->axisX());
    seriesTemperature->attachAxis(temperatureAxis);
    seriesTemperature->setColor(Qt::red);

    ui->chart->setChart(chart);
    ui->chart->setRenderHint(QPainter::Antialiasing);

    modbusManager = new ModbusManager();
    modbusManager->moveToThread(&modbusThread);
    connect(modbusManager, &ModbusManager::updateChart, this, &MainWindow::updateChart);
    connect(modbusManager, &ModbusManager::updateProcessedSeries, this, &MainWindow::updateProcessedSeries);
    connect(modbusManager, &ModbusManager::updateTemperatureSeries, this, &MainWindow::updateTemperatureSeries);
    connect(modbusManager, &ModbusManager::updateStatusBar, this, &MainWindow::updateStatusBar);
    connect(this, &MainWindow::start, modbusManager, &ModbusManager::start);
    connect(this, &MainWindow::stop, modbusManager, &ModbusManager::stop);
    connect(&modbusThread, SIGNAL (started()), modbusManager, SLOT (connectToDevice()));
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

void MainWindow::clearSeries()
{
    axisX->setRange(0, 30.0);
    seriesX->clear();
    seriesY->clear();
    seriesZ->clear();
    seriesVelocityX->clear();
    seriesVelocityY->clear();
    seriesVelocityZ->clear();
    seriesTemperature->clear();
    seriesRMSX->clear();
    seriesRMSY->clear();
    seriesRMSZ->clear();
    seriesVRMSX->clear();
    seriesVRMSY->clear();
    seriesVRMSZ->clear();
    seriesPPX->clear();
    seriesPPY->clear();
    seriesPPZ->clear();
    seriesKurtX->clear();
    seriesKurtY->clear();
    seriesKurtZ->clear();
}

void MainWindow::clearChart()
{
    clearSeries();
    foreach (QAbstractSeries *series, chart->series()) {
        chart->removeSeries(series);
    }
    foreach (QAbstractAxis *axis, chart->axes()) {
        chart->removeAxis(axis);
    }
}

void MainWindow::setChartA()
{
    clearChart();
    chart->addSeries(seriesX);
    chart->addSeries(seriesY);
    chart->addSeries(seriesZ);
    chart->addSeries(seriesVelocityX);
    chart->addSeries(seriesVelocityY);
    chart->addSeries(seriesVelocityZ);
    chart->addSeries(seriesTemperature);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisRaw, Qt::AlignLeft);
    chart->addAxis(axisVelocity, Qt::AlignLeft);
    chart->addAxis(temperatureAxis, Qt::AlignRight);
    seriesX->attachAxis(chart->axisX());
    seriesX->attachAxis(axisRaw);
    seriesY->attachAxis(chart->axisX());
    seriesY->attachAxis(axisRaw);
    seriesZ->attachAxis(chart->axisX());
    seriesZ->attachAxis(axisRaw);
    seriesVelocityX->attachAxis(chart->axisX());
    seriesVelocityX->attachAxis(axisVelocity);
    seriesVelocityY->attachAxis(chart->axisX());
    seriesVelocityY->attachAxis(axisVelocity);
    seriesVelocityZ->attachAxis(chart->axisX());
    seriesVelocityZ->attachAxis(axisVelocity);
    seriesTemperature->attachAxis(chart->axisX());
    seriesTemperature->attachAxis(temperatureAxis);
//    seriesTemperature->setColor(Qt::red);
}

void MainWindow::setChartB()
{
    clearChart();

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
}

void MainWindow::on_radio_live_raw_toggled(bool checked)
{
    toggleGroupBox(ui->group_live_raw, checked);
    if (checked){
        setChartA();
        mode = 0;
    }
}

void MainWindow::on_radio_live_proc_toggled(bool checked)
{
    toggleGroupBox(ui->group_live_proc, checked);
    if (checked){
        setChartB();
        mode = 1;
    }
}

void MainWindow::on_radio_record_raw_toggled(bool checked)
{
    toggleGroupBox(ui->group_record_raw, checked);
    if (checked){
        setChartA();
        mode = 2;
    }
}

void MainWindow::on_radio_record_proc_toggled(bool checked)
{
    toggleGroupBox(ui->group_record_proc, checked);
    if (checked){
        setChartB();
        mode = 3;
    }
}

void MainWindow::on_button_start_clicked()
{
    ui->button_start->setEnabled(false);
    ui->button_stop->setEnabled(true);
    clearSeries();
    statusBar()->showMessage(QString("Started..."));
    if (mode == 0 || mode == 1) {
        emit start(mode, ui->combo_live_all->currentIndex(), ui->spin_live->value(), 0);
    } else if (mode == 2) {
        outputFile.setFileName("signal_" + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss") + ".txt");
        outputFile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&outputFile);
        out << "Time;Acceleration_X;Acceleration_Y;Acceleration_Z;Velocity_X;Velocity_Y;Velocity_Z\n";
        emit start(mode, ui->combo_record_all->currentIndex(), ui->spin_record->value(), ui->spin_record_time_raw->value());
    } else if (mode == 3) {
        outputFile.setFileName("statistic_" + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss") + ".txt");
        outputFile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&outputFile);
        out << "Time;RMS_X;RMS_Y;RMS_Z;VRMS_X;VRMS_Y;VRMS_Z;PP_X;PP_Y;PP_Z;Kurtosis_X;Kurtosis_Y;Kurtosis_Z\n";
        emit start(mode, ui->combo_record_all->currentIndex(), ui->spin_record->value(), ui->spin_record_time_proc->value());
    }
}

void MainWindow::on_button_stop_clicked()
{
    ui->button_start->setEnabled(true);
    ui->button_stop->setEnabled(false);
    emit stop();
}

void MainWindow::updateChart(QList<QPointF> dataX, QList<QPointF> dataY, QList<QPointF> dataZ, bool rawOrVel)
{
    if (mode == 0) {
        qreal counter = dataX.last().x();
        if (!rawOrVel) {
            if (ui->check_live_x->isChecked()) seriesX->append(dataX);
            if (ui->check_live_y->isChecked()) seriesY->append(dataY);
            if (ui->check_live_z->isChecked()) seriesZ->append(dataZ);
        } else {
            if (ui->check_live_x->isChecked()) seriesVelocityX->append(dataX);
            if (ui->check_live_y->isChecked()) seriesVelocityY->append(dataY);
            if (ui->check_live_z->isChecked()) seriesVelocityZ->append(dataZ);
        }
        if (counter > xSeriesLength) {
            chart->axisX()->setRange(counter - xSeriesLength, counter);
            if (!rawOrVel) {
                if (ui->check_live_x->isChecked()) seriesX->removePoints(0, 20);
                if (ui->check_live_y->isChecked()) seriesY->removePoints(0, 20);
                if (ui->check_live_z->isChecked()) seriesZ->removePoints(0, 20);
            } else {
                if (ui->check_live_x->isChecked()) seriesVelocityX->removePoints(0, 20);
                if (ui->check_live_y->isChecked()) seriesVelocityY->removePoints(0, 20);
                if (ui->check_live_z->isChecked()) seriesVelocityZ->removePoints(0, 20);
            }
        }
    } else if (mode == 2) {
        if (dataX.last().x() >= ui->spin_record_time_raw->value() - 0.000625) {  // Fighting with floats.
            on_button_stop_clicked();
            outputFile.close();
            statusBar()->showMessage(QString("File saved!"));
            return;
        }
        QTextStream out(&outputFile);
        if (!rawOrVel) {
            for (int i = 0; i < dataX.length(); ++i) {
                out << dataX.at(i).x() << ";" << dataX.at(i).y() << ";" << dataY.at(i).y() << ";" << dataZ.at(i).y() << ";;;\n";
            }
        } else {
            for (int i = 0; i < dataX.length(); ++i) {
                out << dataX.at(i).x() << ";;;;" << dataX.at(i).y() << ";" << dataY.at(i).y() << ";" << dataZ.at(i).y() << "\n";
            }
        }
    }
}

void MainWindow::updateProcessedSeries(QList<QPointF> samples)
{
    if (mode == 1) {
        qreal counter = samples.last().x();
        if (ui->check_live_rms->isChecked()) {
            seriesRMSX->append(samples.at(0));
            seriesRMSY->append(samples.at(1));
            seriesRMSZ->append(samples.at(2));
        }
        if (ui->check_live_vrms->isChecked()) {
            seriesVRMSX->append(samples.at(3));
            seriesVRMSY->append(samples.at(4));
            seriesVRMSZ->append(samples.at(5));
        }
        if (ui->check_live_pp->isChecked()) {
            seriesPPX->append(samples.at(6));
            seriesPPY->append(samples.at(7));
            seriesPPZ->append(samples.at(8));
        }
        if (ui->check_live_kurt->isChecked()) {
            seriesKurtX->append(samples.at(9));
            seriesKurtY->append(samples.at(10));
            seriesKurtZ->append(samples.at(11));
        }
        if (counter > xSeriesLength) {
            chart->axisX()->setRange(counter - xSeriesLength, counter);
            if (ui->check_live_rms->isChecked()) {
                seriesRMSX->removePoints(0, 1);
                seriesRMSY->removePoints(0, 1);
                seriesRMSZ->removePoints(0, 1);
            }
            if (ui->check_live_vrms->isChecked()) {
                seriesVRMSX->removePoints(0, 1);
                seriesVRMSY->removePoints(0, 1);
                seriesVRMSZ->removePoints(0, 1);
            }
            if (ui->check_live_pp->isChecked()) {
                seriesPPX->removePoints(0, 1);
                seriesPPY->removePoints(0, 1);
                seriesPPZ->removePoints(0, 1);
            }
            if (ui->check_live_kurt->isChecked()) {
                seriesKurtX->removePoints(0, 1);
                seriesKurtY->removePoints(0, 1);
                seriesKurtZ->removePoints(0, 1);
            }
        }
    } else if (mode == 3) {
        if (samples.last().x() >= ui->spin_record_time_proc->value() - 0.05) {  // Fighting with floats.
            on_button_stop_clicked();
            outputFile.close();
            statusBar()->showMessage(QString("File saved!"));
            return;
        }
        QTextStream out(&outputFile);
        out << samples.at(0).x();
        for (int i = 0; i < samples.length(); ++i) {
            out << ";" << samples.at(i).y();
        }
        out << "\n";
    }
}

void MainWindow::updateTemperatureSeries(qreal x, qreal y)
{
    if (ui->check_live_temp->isChecked()) {
        seriesTemperature->append(x, y);
        if (x > xSeriesLength) {
            seriesTemperature->removePoints(0, 1);
        }
    }
}

void MainWindow::updateStatusBar(quint16 fifoOverrunAdxl, quint16 fifoOverrunRawA, quint16 fifoOverrunRawB, quint16 fifoOverrunVelocity, quint16 fifoOverrunProcessed)
{
    statusBar()->showMessage(QString("FIFO Overrun. ADXL: %1   RawA: %2   RawB: %3   Velocity: %4   Processing: %5").arg(QString::number(fifoOverrunAdxl)).arg(QString::number(fifoOverrunRawA)).arg(QString::number(fifoOverrunRawB)).arg(QString::number(fifoOverrunVelocity)).arg(QString::number(fifoOverrunProcessed)));
}
