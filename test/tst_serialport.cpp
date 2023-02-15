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
   QSerialPort tango;
void Tst_serialport::sanityCheck(){
     // raw,echo=0,b115200
    socat.startDetached("socat -d -d -d -d pty,raw,echo=0,b115200,link=/tmp/banana,  pty,raw,echo=0,b115200,link=/tmp/tango");
    sleep(2);

    _store = new store("/tmp/banana");

    QCOMPARE(_store->dev,"/tmp/banana");
	_store->~store();
}
void Tst_serialport::checkSendMessage(){

    tango.setPortName("/tmp/tango");
    tango.setBaudRate(QSerialPort::Baud115200);
    tango.setDataBits(QSerialPort::Data8);
    tango.setStopBits(QSerialPort::OneStop);
    tango.setParity(QSerialPort::NoParity);
    tango.setFlowControl(QSerialPort::NoFlowControl);
    tango.open(QIODevice::WriteOnly);

	tango.write("Hello");
	
     tango.waitForBytesWritten();

	 QSerialPort port;
	 port.setPortName("/tmp/banana");
	 port.setBaudRate(QSerialPort::Baud115200);
	 port.setDataBits(QSerialPort::Data8);
	 port.setStopBits(QSerialPort::OneStop);
	 port.setParity(QSerialPort::NoParity);
	 port.setFlowControl(QSerialPort::NoFlowControl);
	 port.open(QIODevice::ReadOnly);
	 port.waitForReadyRead(1000);
	 QString message = port.readAll();
	 tango.close();
	 port.close();

     QCOMPARE(message,"Hello");
	
}
void Tst_serialport::storeMessage(){
	_store = new store("/tmp/banana");
	tango.setPortName("/tmp/tango");
	tango.setBaudRate(QSerialPort::Baud115200);
	tango.setDataBits(QSerialPort::Data8);
	tango.setStopBits(QSerialPort::OneStop);
	tango.setParity(QSerialPort::NoParity);
	tango.setFlowControl(QSerialPort::NoFlowControl);
	tango.open(QIODevice::WriteOnly);

	tango.write("Hello");
	
	 tango.waitForBytesWritten();
	 _store->forceRead(5);
	 tango.close();
	 QString message = _store->serialLog;
	 QCOMPARE(message,"Hello");
}


void Tst_serialport::closeHandle(){
	
    socat.close();
	_store->~store();
}
QTEST_MAIN(Tst_serialport)
