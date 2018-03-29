#include <iostream>
#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
using namespace std;
int main(){
    Server * s = new Server();
    s->init();
    vector<double> v = s->generateOneSecondSignal(-1);//config.CHUNK_SIZE/config.PERIOD by default
    s->sendSomeSignal(v);

    delete(s);
    return 0;
}

