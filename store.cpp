#include "store.h"
#include <cstdio>

#include <QCoreApplication>
#include <qobject.h>

int store::setupSerial() {
	
    QSerialPort* serial= new QSerialPort();
    serial->setPortName(this->dev);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (!serial->open(QIODevice::ReadOnly)) {
        //emit error(tr("Can't open %1, error code %2").arg(this->dev).arg(port->error()));
        std::cout << "welp you fucked up";
        return 1;
    }
	
    //connect(&m_timer, &QTimer::timeout, this, &SerialPortReader::handleTimeout);

	/*while (serial->waitForReadyRead()) {
		QByteArray serialLog=serial->readAll();
	}*/

    this->port = serial;
    connect(this->port, &QSerialPort::readyRead, this, &store::handleReadyRead);
    connect(this->port, &QSerialPort::errorOccurred, this, &store::handleError);
	

	return 0;	
}


store::store( char * dev, QObject *parent  ): QObject(parent){
	//default device

	if (dev == nullptr){
        int len = sizeof(char)*strlen(DEFAULT_DEVICE);
        this->dev = (char*)malloc(len);
        strcpy(this->dev,DEFAULT_DEVICE);
	}

	setupSerial();
	
}
//destructor for store 
store::~store(){
	closeSerial();
	//free up device
	free(this->dev);

}


void store::handleReadyRead(){
	lastMessage=port->readAll(); 
	qDebug()<< lastMessage; 
	serialLog.append(lastMessage);
}
void store::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        qDebug() << QObject::tr("An I/O error occurred while reading "
                                        "the data from port %1, error: %2")
                            .arg(port->portName())
                            .arg(port->errorString())
                         << "\n";
        QCoreApplication::exit(1);
    }
}


int store::closeSerial(){
	// TODO: Error handling?
	int i =0; 
	port->close();	
	return i;
}
