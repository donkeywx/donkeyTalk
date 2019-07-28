#include <iostream>
#include <stdio.h>
#include "netlib.h"

using namespace std;

int main(int argc, char** argv)
{

    printf("start\n");

    Netlib::listen("127.0.0.1", 9088);

    Netlib::startEventLoop(10);
    
    return 0;
}