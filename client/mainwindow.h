#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mytcpsocket.h"
#include <fftw3.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MyTcpSocket * socketConnection;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
