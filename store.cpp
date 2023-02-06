#include "store.h"

store::store(char * dev){
	//default device
	if (dev == nullptr){
        int len = sizeof(char)*strlen(DEFAULT_DEVICE);
        this->dev = (char*)malloc(len);
        strncpy(this->dev,DEFAULT_DEVICE, len);
	}
	setupSerial();
	this->mudança=0; 
	
}
//destructor for store 
store::~store(){
	closeSerial();
	//free up device
	free(this->dev);

}

int store::setupSerial(){
	QSerialPort port(this->dev);
	this->port = &port;

    port.setBaudRate(QSerialPort::Baud115200);
    port.setDataBits(QSerialPort::Data8);
    port.setStopBits(QSerialPort::OneStop);
    port.setParity(QSerialPort::NoParity);
    port.setFlowControl(QSerialPort::NoFlowControl);

	QObject::connect(&port, &QSerialPort::readyRead,
                     [&port](){
        qDebug() << port.readAll();
    });

	QObject::connect(&port, &QSerialPort::errorOccurred,
                     [](QSerialPort::SerialPortError error){
        qDebug() << error;
    });



	return 0;

}
int store::closeSerial(){
	// TODO: Error handling?
	int i =0; 
	this->port->close();	
	return i;
}
