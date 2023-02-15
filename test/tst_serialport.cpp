#include "tst_serialport.h"
#include <cstdio>
#include <iostream>
#include <regex>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <fstream>
#include <QSerialPort>

using namespace std;
void Tst_serialport::sanityCheck(){
     // raw,echo=0,b115200
    socat.startDetached("socat -d -d pty,raw,echo=0,b115200,link=/tmp/banana,  pty,raw,echo=0,b115200,link=/tmp/tango");
    sleep(4);
    
    _store = new store("/tmp/banana");

    QCOMPARE(_store->dev,"/tmp/banana");
}
void Tst_serialport::checkSendMessage(){
    QSerialPort tango;
	tango.setPortName("/tmp/tango");
	tango.setBaudRate(QSerialPort::Baud115200);
    tango.setDataBits(QSerialPort::Data8);
	tango.setStopBits(QSerialPort::OneStop);
	tango.setParity(QSerialPort::NoParity);
	tango.setFlowControl(QSerialPort::NoFlowControl);
    tango.open(QIODevice::ReadWrite);
    tango.write("Hello");
    _store->forceRead();

    tango.waitForBytesWritten();

    //_store->handleReadyRead();
    tango.close();
    sleep(2);

    QCOMPARE(_store->lastMessage,"Hello");
	
}


void Tst_serialport::closeHandle(){
    socat.close();
    _store->~store();
}
QTEST_MAIN(Tst_serialport)
