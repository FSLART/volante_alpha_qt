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
void Tst_serialport::setup(){
	program="socat";
	args = QStringList();

	args.append("pty,raw,echo=0,b38400,link=/tmp/banana,");
	args.append("pty,raw,echo=0,b38400,link=/tmp/tango");
}
void Tst_serialport::sanityCheck(){
     //-d -d -d -d
    socat.startDetached(program, args);
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
	 port.setBaudRate(QSerialPort::Baud38400);
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
/*void Tst_serialport::debugFailing(){
	_store->lastMessage.clear();
	_store->setRpm(0);
	tangoWriteSetup();
    char msg[]= {'\x20','\xF9','\x04','\x9D','\x56','\x05','\x66','\x81','\x86','\x7F','\xA9','\xA0','\xF1','\x77','\xAA','\x13','\xAD','\x2F','\x8B','\x47','\x21','\xA3','\x88','\x54','\x19','\xD4','\x7C','\xFA','\x8A','\x26','\xB0','\x56','\x59','\x22','\xDD','\x3C','\x8E','\x92','\x11','\xE2','\x18','\x14','\xCC','\xE1','\x73','\x77','\x23','\xF1','\x9E','\x45','\x09','\xD2','\xAB','\x34','\x51','\x88','\x07','\xFB','\x0B','\xA9','\x4C','\x1E','\x84','\xDC','\x7E','\x67','\x59','\xA3','\x00','\xA1','\x0A','\xB1','\x48','\x5C','\x39','\x8E','\xCF','\xA8','\x36','\x96','\x23','\x19','\x30','\x6B','\x38','\x8B','\x9F','\xCF','\x53','\x3E','\x3B','\x55','\x4A','\x19','\xA7','\x2C','\x6F','\xED','\x4A','\xEA','\xA2','\x96','\x1D','\xFB','\x1A','\xDB','\x1E','\xB7','\x23','\x55','\x1D','\x58','\x52','\x1C','\x6C','\x6D','\x58','\x57','\xE5','\xD4','\xCA','\xEC','\xD0','\x52','\xCA','\x16','\xC5','\x70','\xF3','\x63','\x73','\xE8','\xBD','\xA0','\x23','\xE1','\x23','\xF0','\xD9','\x03','\xA1','\x04','\xBE','\x52','\x6B','\x59','\x1C','\x0D','\x4B','\x49','\xB8','\x67','\x13','\xAA','\x17','\x4F','\x9A','\x87','\xC5','\xDF','\xC9','\x80','\xCF','\xEE','\xE3','\x31','\xB2','\x35','\x21','\xAF','\xCA','\xB1','\x10','\x82','\x37','\x63','\x3A','\xEB','\x4E','\xA9','\x67','\x8F','\x12','\xE0','\x1F','\x2F','\xE3','\xD2','\xBD','\x10','\x06','\xCD','\xDF','\x4D','\x25','\xC1','\xBF','\x3D','\x0B','\xAB','\x60','\xB4','\x93','\xAB','\xB8','\x86','\x2B','\x03','\xAC','\x1B','\x78','\x3E','\x39','\x8D','\xC6','\x79','\x3F','\x26','\x36','\x04','\x6A','\x03','\x4C','\x1F','\xD5','\x8C','\xA7','\x0E','\x56','\xF8','\x9A','\x3F','\x43','\xEA','\xCA','\x90','\x32','\xAB','\xC9','\xB0','\x01','\x4A','\x7F','\x99','\x64','\x64','\x31','\x8B','\x65','\xC7','\xBA','\x58','\x26','\xEF','\x85','\x76','\x51','\x4A','\xB6','\x8F','\x2F','\x82','\xCD','\xF5','\x8A','\xBA','\x4F','\x3D','\xFC','\x68','\x7C','\xC3','\xD9','\x4F','\x99','\x97','\x06','\x47','\xFE','\xE3','\x48','\xB7','\xCF','\x44','\x78','\x1C','\xBD','\x75','\x20','\xA5','\x95','\x89','\xB1','\x57','\xE6','\x97','\x7A','\x8F','\x6D','\x23','\x08','\x78','\x42','\x20','\x77','\xA7','\x76','\xD5','\x8A','\x4A','\xAF','\xA9','\xF9','\x7A','\xC2','\x30','\x17','\x1A','\x9C','\x41','\x76','\xB1','\x0D','\x2F','\x9C','\xFD','\xB7','\x82','\xDB','\xCE','\x97','\x5D','\x27','\xB0','\x62','\x55','\x8C','\x7C','\x84','\x7A','\x49','\xE0','\x02','\x3C','\x1D','\x9A','\xE4','\x9A','\x1A','\xB7','\x86','\xA4','\x67','\xF5','\x4E','\x76','\x0B','\x1E','\x98','\xA7','\x9C','\xDB','\x95','\x22','\xDF','\x70','\x9D','\xF3','\x9A','\xF4','\xAF','\x01','\xC6','\x27','\x86','\x4D','\x84','\x3C','\x6E','\x87','\x1D','\x0A','\x18','\xA3','\xF6','\xFB','\x07','\x34','\xE2','\xBB','\x26','\x16','\xA2','\x55','\xC0','\xD4','\xD5','\xEC','\x8A','\x56','\x42','\x43','\xB8','\x0F','\x6F','\x86','\x0B','\xFD','\xCC','\xE1','\x80','\xFB','\x01','\xDF','\x3C','\x2F','\x22','\x5F','\x79','\xAD','\x51','\xD9','\xDA','\x4C','\xCB','\x88','\x99','\x80','\x8F','\x94','\x08','\x84','\x95','\x90','\x30','\x3F','\x10','\xAF','\x9B','\xB9','\x28','\x98','\x43','\xBE','\x1C','\x9A','\xCA','\xCE','\x5A','\xE1','\x1B','\x46','\x1C','\xB8','\x0F','\x15','\xB1','\x9F','\x47','\x09','\x9C','\xF9','\x3B','\x05','\xB4','\x2D','\x65','\xC9','\x79','\x8D','\xBE','\xC5','\x13','\xB9','\x19','\x81','\xCF','\xA0','\xD9','\x75','\x8C','\xD9','\x82','\x3E','\xD5','\x3E','\x89','\xE3','\xD8','\xEC','\x6E','\x6D','\xA8','\x77','\x12','\x09','\x8B','\xD8','\x83','\xD7','\x4A','\x31','\x0C','\xFA','\x18','\x4F','\x61','\x53','\x34','\xFA','\xCF','\xCC','\x3B','\xF8','\x6C','\xD2','\xA2','\xC1','\xA1','\x28','\xB7','\x5A','\x8D','\xD2','\x4A','\x64','\x42','\xB2','\x08','\xBB','\x75','\xBB','\x72','\x6E','\xC0','\xE0','\x50','\x29','\xEE','\x03','\xDE','\x98','\xB1','\xA2','\x5F','\x1A','\xDE','\xFA','\x3D','\xA6','\x9E','\x06','\xFD','\x6B','\xC8','\x0A','\xA0','\x2F','\xDB','\xE1','\xEE','\x0E','\x56','\x7B','\xEF','\x1A','\x74','\xB7','\xD5','\x1F','\x26','\x3A','\xED','\x42','\xAE','\x9C','\x1F','\xF7','\x09','\x2D','\xA2','\x9C','\xA0','\x60','\x8C','\x0B','\x25','\xA4','\x33','\x73','\xAF','\x55','\xCC','\xD9','\xC0','\x42','\xD5','\x83','\x02','\x45','\xC0','\x9D','\x69','\xF2','\x6D','\xD3','\xAE','\x1C','\xA7','\xEE','\x03','\x26','\x3B','\x0F','\x14','\x9A','\x9F','\x9F','\x55','\x90','\xFD','\x42','\x4C','\x68','\x32','\xAC','\xE9','\x5F','\xC3','\x5A','\xE2','\x20','\x7E','\xBC','\x18','\xC4','\x66','\x1C','\xCC','\x78','\xBE','\x2E','\x0F','\xD2','\x7D','\x66','\x79','\x1E','\x69','\xB2','\xB9','\x93','\xED','\x9F','\xC4','\x6D','\x88','\xA1','\x29','\xBE','\x59','\x02','\xE2','\x47','\xD4','\xED','\x51','\x7B','\xFE','\x30','\x0C','\x32','\x99','\xED','\xD0','\x62','\x4A','\x48','\x3C','\xB8','\x65','\xB4','\xC7','\x74','\x89','\x73','\x28','\xBF','\x40','\x58','\x76','\x4F','\xAB','\x09','\x01','\x18','\xA5','\xAE','\x18','\x1E','\x3A','\x94','\x21','\x59','\xD4','\x7F','\xFE','\xB5','\x4C','\xC0','\x4D','\xA7','\x18','\xF0','\x3E','\xED','\x83','\xDE','\xB1','\xF4','\xDA','\xB2','\xE8','\x62','\x54','\xE3','\xCC','\xED','\xDF','\x16','\x3A','\x91','\x7D','\xD1','\xE3','\x0B','\x7A','\xDA','\xAA','\x3D','\xE3','\x13','\x48','\x52','\xBB','\x31','\x30','\xA1','\x94','\x6B','\xD7','\x22','\xCD','\x4F','\x1B','\x6B','\x6B','\xAA','\x31','\x3A','\xA1','\x5B','\xCD','\x09','\x41','\xDC','\x43','\xD8','\x1A','\xB5','\x3F','\x8F','\xDD','\x8C','\xFB','\x4D','\x14','\x3C','\x5E','\x04','\xF8','\x73','\xD7','\x10','\xE7','\x36','\x14','\x3A','\x3B','\xB5','\x75','\xE2','\xBC','\x70','\x17','\x37','\x2D','\xFC','\x63','\xDE','\x43','\x66','\x54','\x7C','\x5D','\x9E','\x0A','\x8C','\x26','\x26','\x2D','\x46','\xAA','\x89','\x9D','\x52','\x0B','\x54','\x8F','\x3B','\xB3','\xFA','\x39','\xDF','\x3E','\xC7','\x90','\xBB','\x93','\x59','\x2A','\x9C','\xF2','\x77','\x32','\x42','\x4D','\x71','\x99','\x45','\x37','\x74','\x8E','\xE0','\xE7','\xC4','\xE7','\xCB','\x94','\x26','\xBA','\x20','\x5B','\xF0','\xA8','\xC8','\xBA','\x97','\x28','\xD7','\x82','\x58','\x3C','\x5B','\xEA','\x1C','\x94','\x78','\x38','\x45','\x1D','\xCB','\x9A','\x00','\x49','\x0D','\x76','\x9A','\xC7','\xDE','\xEE','\x8A','\xFD','\x75','\x8F','\xCA','\xC3','\x9D','\xE5','\x88','\xC3','\x94','\xB0','\xD7','\x9D','\x78','\xCA','\x3E','\x7C','\xE3','\x83','\xE6','\x45','\xFC','\x5C','\x85','\x03','\xD9','\x2F','\x79','\xB0','\xA8','\xD8','\x04'};
	//random amount from 0 to 1000
	
	for(int i = 0; i < sizeof(msg); i++){
		//write a random byte from 0 to 255
		//the chance of landing 0xFF 4 times in a row is 1/256^4 or 1/65536, which is very unlikely, theres also a chance of landing an ff prepending the actual message 
        tango.putChar(msg[i]);
		tango.waitForBytesWritten();
	}
	tango.write(rpm_message, 18);
	tango.waitForBytesWritten();
	tango.close();
	_store->port->waitForReadyRead();
	QCOMPARE(_store->getRpm(),10000);
}*/
void Tst_serialport::prependingTrash(){
	//open a unbuffered file to write to
    _store->lastMessage.clear();
	_store->setRpm(0);
	tangoWriteSetup();

	//random amount from 0 to 1000
	int random = instance.randomInt(0,1000);
	for(int i = 0; i < random; i++){
		//write a random byte from 0 to 255
		//the chance of landing 0xFF 4 times in a row is 1/256^4 or 1/65536, which is very unlikely, theres also a chance of landing an ff prepending the actual message 
		char t =  (char)instance.randomInt(0,255);
		tango.putChar(t);
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
	int random = instance.randomInt(0,1000);
	for(int i = 0; i < random; i++){
		//write a random byte from 0 to 255
		//the chance of landing 0xFF 4 times in a row is 1/256^4 or 1/65536, which is very unlikely, theres also a chance of landing an ff prepending the actual message 
		tango.putChar((char)instance.randomInt(0,255));
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
	char * rpm_message2 = new char[18];
	memcpy(rpm_message2,rpm_message,18);
	int a = instance.randomInt(0, 10000);
	rpm_message2[13]= (char)(a & 0xFF);
	rpm_message2[14]= (char)((a >> 8) & 0xFF);
	rpm_message2[15]= (char)((a >> 16) & 0xFF);
	rpm_message2[16]= (char)((a >> 24) & 0xFF);
	

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
	tango.setBaudRate(QSerialPort::Baud38400);
	tango.setDataBits(QSerialPort::Data8);
	tango.setStopBits(QSerialPort::OneStop);
	tango.setParity(QSerialPort::NoParity);
	tango.setFlowControl(QSerialPort::NoFlowControl);
	tango.open(QIODevice::WriteOnly);

}
