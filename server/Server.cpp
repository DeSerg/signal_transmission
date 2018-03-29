//
// Created by cloudy on 14.03.18.
//

#include <sstream>
#include <cstring>
#include "Server.h"

// Server::Server() {}
// Server::~Server() {}

/// не самое лучшее названия для метода) чтобы понять, что он делает, нужно его читать, а это лишняя трата времени
void Server::method() {
    char buf[1024];
    int bytes_read;

    //connection queue has size of 1
    listen(serverSocketFileDescriptor, 1);

    /// while(1) { - 1 преобразовывается к true, можно избежать неявного преобразования
    while (true) {
        
        /// желательно прочитать codestyle какой-нибудь большой компании,
        /// например Google (https://google.github.io/styleguide/cppguide.html),
        /// и следовать ему везде. Или если работаешь в компании со своим кодстайлом, соответственно, следовать ему.
        /// Насчет строчки ниже - операторы надо отделять пробелами для улучшения читабельности
        /// int connection=-1; 
        int connection = -1;

        //making a connection from SSFD
        if((connection = accept(serverSocketFileDescriptor, NULL, NULL)) < 0) {
            perror("Server connection accept error:\n\t");
            exit(3);
        }

        while(1){//buf not cleared so message'll be shown twice

            //bytes_read left to scan
            bytes_read = recv(connection, buf, 1024, 0);//from, to, how long, by default
            cout<<"SERVER GET::\t"<<buf<<endl;
            if(bytes_read <= 0) break;
            send(connection, buf, bytes_read, 0);
            cout<<"SERVER SEND::\t"<<buf<<endl;

        }
        //closing only connection, not socket
        close(connection);
    }
}

void Server::init() {
    //set up conf
    config.CHUNK_SIZE = 1024;
    config.PORT=30000;
    config.HOST = AF_INET;
    config.PERIOD = 1.0;

    //Creation FD. It'll be internet socket (not unix), using TCP and with default flags
    if((serverSocketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0))< 0){
        perror("Server socket init error:\n\t");
        exit(1);
    }

    //Configure connection
    serverAddress.sin_family = config.HOST;
    serverAddress.sin_port = htons(config.PORT);//port. Port fits to short, so "htonS")))
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);//everyone can connect

    //Making connection. Struct sockaddr_in(serverAddress) is more useful than pure sockaddr, but must be conversed
    if(bind(serverSocketFileDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        perror("Server bind error:\n\t");
        close(serverSocketFileDescriptor);
        exit(2);
    }
}

double Server::signalFunction(int sequenceElement, int measureRate, int signalFrequency){
    double cos1, cos05, cos3, cos4, noize;
    cos05=sin(signalFrequency*M_PI/measureRate*sequenceElement);
    cos1=cos(signalFrequency*2*M_PI/measureRate*sequenceElement);
    cos3=cos(signalFrequency*3*M_PI/measureRate*sequenceElement);
    cos4=cos(signalFrequency*4*M_PI/measureRate*sequenceElement);
    noize = (rand()%100)*0.0005;
    //rect signal
//    if(sequenceElement<1000 && sequenceElement>500)
//        return 1.0*sin1;
    return cos1*cos3*cos4+noize;
}

vector<double> Server::generateOneSecondSignal(int measureRate){
    if (measureRate==-1){
        measureRate=this->config.CHUNK_SIZE/config.PERIOD;
    }
    int FREQ = 16;//generated signal frequency
    vector<double> signal;
    signal.resize((int)measureRate);
    srand(time(0));
    for(int i=0; i<signal.size(); i++){
        signal[i]=signalFunction(i, measureRate, FREQ );
    }
    //DEBUG
    for(int i = 0; i<signal.size(); i++){
        cout<<signal[i]<<"\t";i++;
        cout<<signal[i]<<"\t";i++;
        cout<<signal[i]<<"\t";i++;
        cout<<signal[i]<<"\t";i++;
        cout<<signal[i]<<"\n";
    }
    return signal;

}

void Server::sendSomeSignal(vector<double> signal) {

    //connection queue has size of 1
    listen(serverSocketFileDescriptor, 1);

    int connection=-1;
    //making a connection from SSFD
    if((connection = accept(serverSocketFileDescriptor, NULL, NULL)) < 0){
        perror("Server connection accept error:\n\t");
        exit(3);
    }

    //convvert data to string to correctly transmit in bytes
    char digits[50];
    for (int i =0; i<signal.size(); i++) {
        //converting to chars to send it correctly
        string str = to_string(signal[i]);
        strcpy(digits, str.c_str());
        send(connection, digits, sizeof(digits), 0);
        cout<<" SERVER: I send\t"<<i<<"\t"<<digits<<endl;
    }

    //closing only connection, not socket
    close(connection);
    close(serverSocketFileDescriptor);

}
