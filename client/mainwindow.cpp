#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <algorithm>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socketConnection = new MyTcpSocket();
    socketConnection->init();

//    QVector<double> data_;
//    data_ = socketConnection->receiveChunk();
    QVector<double> signal, time;
    signal = socketConnection->receiveChunk();
    time.resize(signal.size());
    for(int i=0; i<time.size(); i++){
        time[i] = i;
    }
    //Signal graph
    double s_min = *std::min_element(signal.constBegin(), signal.constEnd());
    double s_max = *std::max_element(signal.constBegin(), signal.constEnd());
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(time, signal);
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    ui->customPlot->xAxis->setRange(0.0, signal.size()*1.0);
    ui->customPlot->yAxis->setRange(s_min, s_max);
    ui->customPlot->replot();


    //making FT
    QVector<double> omega, amplitude;
    amplitude = socketConnection->getSpectre(signal);
    omega.resize(amplitude.size());
    for(int i =0;i<omega.size(); i++){
        omega[i]=i-omega.size()/2;
    }

    //Spectre graph
    ui->spectrePlot->addGraph();
    ui->spectrePlot->graph(0)->setData(omega, amplitude);
    ui->spectrePlot->xAxis->setLabel("omega");
    ui->spectrePlot->yAxis->setLabel("ampl");
    ui->spectrePlot->xAxis->setRange(-1*omega.size(), omega.size()*1);
    ui->spectrePlot->yAxis->setRange(-10, 10);
    ui->spectrePlot->replot();

    //some information about experiment
    ui->textArea->setText("ExperimentParameters:\n");
    QString sss = QString::number(socketConnection->getConfig().CHUNK_SIZE);
    ui->textArea->append("\nSample size:\t"+sss);
    sss=QString::number(socketConnection->getConfig().PERIOD);
    ui->textArea->append("\nTime:\t"+sss);
    sss=QString::number(socketConnection->getConfig().SPECTRE_SIZE);
    ui->textArea->append("\nSpectre size:\t"+sss);
}

MainWindow::~MainWindow()
{
    delete ui;
    socketConnection->~MyTcpSocket();
}
