#include "tst_serialport.h"


using namespace std;
   QSerialPort tango;

   const  char rpm_message[] = {
        (char)0xFF, (char)0xFF, (char)0xFF, (char)0xFF,
        0x0E, 0x00, 0x00, 0x00,
		0x10,
        0x72, 0x70, 0x6D, 0x00,
        0x10, 0x27, 0x00, 0x00,
		0x00};
void Tst_serialport::sanityCheck(){
     //-d -d -d -d
    socat.startDetached("socat pty,raw,echo=0,b115200,link=/tmp/banana,  pty,raw,echo=0,b115200,link=/tmp/tango");
    sleep(2);

    _store = new store("/tmp/banana");

    QCOMPARE(_store->dev,"/tmp/banana");
	_store->~store();
}
void Tst_serialport::checkSendMessage(){
	tangoWriteSetup();
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
	tangoWriteSetup();

	tango.write("World");
	tango.waitForBytesWritten();
	tango.close();
	_store->port->waitForReadyRead();
	 QString message = _store->serialLog;
	 QCOMPARE(message,"World");
}
void Tst_serialport::bsonTest(){
	_store->lastMessage.clear();
    _store->setRpm(0);

	tangoWriteSetup();

	tango.write(rpm_message, 18);
	tango.waitForBytesWritten();
	tango.close();
	_store->port->waitForReadyRead();

	QCOMPARE(_store->getRpm(),10000);
}
//checks if the message still works if it comes in parts
void Tst_serialport::partitionedSlowBsonMessage(){
	qDebug() << "This test checks if the message still works if it comes in parts, simulating a slower transmitting device, as such it will take a few seconds to run";
	_store->lastMessage.clear();
	_store->setRpm(0);
	tangoWriteSetup();
	int count =0; 
	do {
		tango.write(rpm_message+count,3); 
		tango.waitForBytesWritten();
		_store->port->waitForReadyRead();
		count+=3;
	}while (count < 18);
	tango.close();
	_store->port->waitForReadyRead();
	QCOMPARE(_store->getRpm(),10000);
}
void Tst_serialport::prependingTrash(){
	_store->lastMessage.clear();
	_store->setRpm(0);
	tangoWriteSetup();

	//random amount from 0 to 1000
	int random = randomInt(0,1000);
	for(int i = 0; i < random; i++){
		//write a random byte from 0 to 255
		//the chance of landing 0xFF 4 times in a row is 1/256^4 or 1/65536, which is very unlikely, theres also a chance of landing an ff prepending the actual message 
		tango.putChar((char)randomInt(0,255));
		tango.waitForBytesWritten();
	}
	tango.write(rpm_message, 18);
	tango.waitForBytesWritten();
	tango.close();
	_store->port->waitForReadyRead();

	QCOMPARE(_store->getRpm(),10000);
		
}

void Tst_serialport::suffixingTrash(){
	_store->lastMessage.clear();
	_store->setRpm(0);
	tangoWriteSetup();

	//random amount from 0 to 1000
	
	tango.write(rpm_message, 18);
	int random = randomInt(0,1000);
	for(int i = 0; i < random; i++){
		//write a random byte from 0 to 255
		//the chance of landing 0xFF 4 times in a row is 1/256^4 or 1/65536, which is very unlikely, theres also a chance of landing an ff prepending the actual message 
		tango.putChar((char)randomInt(0,255));
		tango.waitForBytesWritten();
	}
	tango.waitForBytesWritten();
	tango.close();
	_store->port->waitForReadyRead();

	QCOMPARE(_store->getRpm(),10000);
		
}
void Tst_serialport::updatingRPM(){
	_store->lastMessage.clear();
	_store->setRpm(0);
	tangoWriteSetup();

	tango.write(rpm_message, 18);
	tango.waitForBytesWritten();
	tango.close();
	_store->port->waitForReadyRead();

	QCOMPARE(_store->getRpm(),10000);
	_store->setRpm(0);
	tangoWriteSetup();
	char *rpm_message2 = (char*)malloc(18);
	memcpy(rpm_message2,rpm_message,18);
	int a = randomInt(0, 10000);
	rpm_message2[10] = (char)(a >> 8);
	rpm_message2[11] = (char)(a & 0xFF);
	

	tango.write(rpm_message2, 18);
	tango.waitForBytesWritten();
	tango.close();
	_store->port->waitForReadyRead();

	QCOMPARE(_store->getRpm(),a);
}

void Tst_serialport::closeHandle(){
	
    socat.close();
	_store->~store();
}
//QTEST_MAIN(Tst_serialport)

//functions

void Tst_serialport::tangoWriteSetup(){
	tango.setPortName("/tmp/tango");
	tango.setBaudRate(QSerialPort::Baud115200);
	tango.setDataBits(QSerialPort::Data8);
	tango.setStopBits(QSerialPort::OneStop);
	tango.setParity(QSerialPort::NoParity);
	tango.setFlowControl(QSerialPort::NoFlowControl);
	tango.open(QIODevice::WriteOnly);

}
int Tst_serialport::randomInt(int offset=0, int n=255){
	QRandomGenerator a = QRandomGenerator::securelySeeded();
	return a.bounded(n)+offset;
}
