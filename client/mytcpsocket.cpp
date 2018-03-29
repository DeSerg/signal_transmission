#include "mytcpsocket.h"
#include <algorithm>

MyTcpSocket::MyTcpSocket(){}
MyTcpSocket::~MyTcpSocket(){
    close(clientSocketFileDescriptor);
}
QVector<double> MyTcpSocket::receiveChunk() {
    vector<double> signal;
    signal.resize(config.CHUNK_SIZE);
    char digits[50];
    string str;
    for(int i =0; i<signal.size(); i++){
        recv(clientSocketFileDescriptor,digits, sizeof(digits), 0);
        //not so optimal, but without locale-conflicts. On the other hand compilator should optimize this code
        string str(digits);
        QString qstr = QString::fromStdString(str);
        double val = qstr.toDouble();
        signal[i]=  val;
    }
    close(clientSocketFileDescriptor);
    QVector<double> signalOutput= QVector<double>::fromStdVector(signal);
    return signalOutput;
}

void MyTcpSocket::init() {
    config.CHUNK_SIZE = 1024;
    config.HOST = INADDR_LOOPBACK;
    config.PORT = 30000;
    config.PERIOD = 1.0;

    //creation of FD. Use internet (not unix) socket, TCP and default flags
    if((clientSocketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0))< 0){
        perror("Client socket init error:\n\t");
        exit(1);
    }

    //configure connection
    connectTo.sin_family = AF_INET;
    connectTo.sin_port = htons(config.PORT);
    connectTo.sin_addr.s_addr = htonl(config.HOST);//127.0.0.1

    //connecting to server. Struct sockaddr_in(connectTo)  is more useful than pure sockaddr, but must be conversed
    if(::connect(clientSocketFileDescriptor, (struct sockaddr *)&connectTo, sizeof(connectTo)) < 0){
        perror("Client connection to server error\n\t");
        exit(2);
    }
}
QVector<double> MyTcpSocket::getSpectre(QVector<double> signal){
    /*spectre sequence length = rate_of_disctretization/number_of_measuremens
      in case of 1-sec signal, it means CHUNK_SIZE=SAMPLE_SIZE=*/
    int REAL = 0;
    int IMAG =1;
    int SIZE = signal.size();
    config.SPECTRE_SIZE =SIZE;

    fftw_complex sample[SIZE];
    fftw_complex spectre[SIZE];
    //creating a plan of transformation
    fftw_plan plan = fftw_plan_dft_1d(SIZE, sample, spectre, FFTW_FORWARD, FFTW_ESTIMATE);//"forward" = t-->omega
    for (int i=0; i<SIZE; i++){
        sample[i][REAL] = signal[i];
        cout<<"SIGNAL[i]:\t"<<sample[i][REAL]<<endl;
        sample[i][IMAG] =0.0;
    }
    fftw_execute(plan);

    double max=0.0;
    double min = 0.0;
    for(int i =0; i<sizeof(spectre)/sizeof(spectre[0]); i++){
        if((spectre[i][REAL])>max)
            max=(spectre[i][REAL]);
        if((spectre[i][REAL])<min)
            min=(spectre[i][REAL]);
    }
    cout<<"MAX "<<max<<endl;

    fftw_destroy_plan(plan);
    fftw_cleanup();
    QVector<double> outputSpectre;
    outputSpectre.resize(sizeof(spectre)/sizeof(spectre[0]));
    for(int i =0; i<sizeof(spectre)/sizeof(spectre[0]); i++){
        outputSpectre[i] = spectre[i][REAL];
//        cout<<i<<"\t\t"<<outputSpectre[i]<<"\t\t"<<spectre[i][REAL]<<"\t\t"<<spectre[i][IMAG]<<"i"<<endl;
    }
    return outputSpectre;

}
configStorage MyTcpSocket::getConfig(){
    return this->config;
}

