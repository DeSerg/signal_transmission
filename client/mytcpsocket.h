/*lib provide connection and ft of received signal*/
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <fftw3.h>

using namespace std;
struct configStorage{
    int SAMPLE_SIZE;//=chunk_size, use not everywhere
    int CHUNK_SIZE;
    double PERIOD;
    int PRICISION;
    int HOST;//AF_INET for example
    string HOST_S;
    int PORT;
    int SPECTRE_SIZE;
};

class MyTcpSocket
{
private:
    configStorage config;
    int clientSocketFileDescriptor;
    struct sockaddr_in connectTo;

public:
    MyTcpSocket();
    ~MyTcpSocket();
    QVector<double> receiveChunk();
    QVector<double> getSpectre(QVector<double> signal);
    void init();
    configStorage getConfig();
};
