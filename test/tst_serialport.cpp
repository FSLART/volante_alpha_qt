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
	
	pid= fork();
	
	if(pid == 0){
        launchSocat();
		
	}else if(pid < 0){
		QFAIL("Failed to fork");
    }else{
		// read /tmp/socatVolante and regex /dev/pts/[0-9]+ the first match equals banana
		// the second match equals tango
		//wait 2 secs
		sleep(2);
		ifstream fp;
        //TODO CAN BE HEAVILY OPTIMIZED BUT I JUST SPENT LIKE AN ENTIRE DAY DUE TO FUCKERY THEY DID WITH C++
		std::string s;
		fp.open("/tmp/socatVolante");

		std::getline(fp, s);

        std::regex r(".*(\\/dev\\/pts\\/\\d+)(.*).*");
		std::smatch m;
		if(std::regex_search(s, m, r)){
			
            banana = QString::fromStdString(m[1]);
			
		}	
		getline(fp, s);
		if(std::regex_search(s, m, r)){
			tango = QString::fromStdString(m[1]);
		}
		//sudo chmod 666
			
		fp.close();

    //system("chmod 666 /dev/pts/*");
		//convert banana QString to char*
		QByteArray ba = banana.toLocal8Bit();
		char *cstr = ba.data();
		

        _store = new store(cstr);
	
        QCOMPARE(_store->dev, cstr);

	}
}
void Tst_serialport::checkSendMessage(){
    //execute echo -n \"hello\" > tango
	//read banana
	//compare banana to hello
	QSerialPort serial;
    serial.setPortName(tango);
	serial.setBaudRate(QSerialPort::Baud115200);
	serial.setDataBits(QSerialPort::Data8);
	serial.setParity(QSerialPort::NoParity);
	serial.setStopBits(QSerialPort::OneStop);
	serial.setFlowControl(QSerialPort::NoFlowControl);
	serial.open(QIODevice::ReadWrite);
    serial.write("hello\0", 5);
    serial.waitForBytesWritten();
    serial.close();
	
    QCOMPARE(_store->lastMessage, "hello");
	
}
void Tst_serialport::launchSocat(){

    char socatCommand[]= "socat -d -d pty,raw,echo=1,b115200 pty,raw,echo=1,b115200 2> /tmp/socatVolante";
	system(socatCommand);

	
}

void Tst_serialport::closeHandle(){
	//kill children 
	kill(pid, SIGTERM);
}
QTEST_MAIN(Tst_serialport)
