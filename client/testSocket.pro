

QT       += core gui network printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testSocket
TEMPLATE = app
LIBS += /usr/local/lib/libfftw3.a


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    mytcpsocket.cpp

HEADERS  += mainwindow.h \
    mytcpsocket.h \
    qcustomplot.h

FORMS    += mainwindow.ui
