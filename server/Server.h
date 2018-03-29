/*USAGE: init - generate - send*/
#ifndef SPECTRE_SERVER_H
#define SPECTRE_SERVER_H
/// сишный способ избавиться от повторного включения хедера, более современный способ - #pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
//#include <alsa/asoundlib.h>
#include <vector>
#include <math.h>

/// зависимости, не требуемые в заголовочном файле, нужно переместить в cpp-шник.
/// это нужно для уменьшения объема исполняемого кода, уменьшения времени компиляции
/// здесь в хедере нужен, кажется, только вектор, все остальное можно перенести в реализацию

using namespace std;

/// Имена классов и структур обычно называются с большой буквы, хотя зависит от кодстайла, конечно
struct configStorage{
    int SAMPLE_SIZE;//no used: =CHUNK_SIZE
    int CHUNK_SIZE;
    double PERIOD;
    int PRICISION; /// PRECISION
    int HOST;//AF_INET for example
    string HOST_S;
    int PORT;
};

class Server
{
private:
    configStorage config;
    struct sockaddr_in serverAddress;
    int serverSocketFileDescriptor;

public:
    /// Пустые конструктор и деструктор генерируются автоматически
    /// Server();
    /// ~Server();

    void method();
    void init(); /*30000 port, listen everybody, but only one*/
    void sendSomeSignal(vector<double> signal);
    vector<double> generateOneSecondSignal(int measureRate);

    double signalFunction(int sequenceElement, int measureRate, int signalFrequency);
};

#endif //SPECTRE_SERVER_H
/// хороший стиль - оставлять пустую строку в конце файла
