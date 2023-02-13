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


//getters and setters
int store::getRpm() const{
	return this->m_rotationsPerMinute;

}
int store::getGearShift() const{
	return this->m_gearShift;
}
int store::getEngineTemperature() const{
	return this->m_engineTemperature;
}
float store::getOilPressure() const{
	return this->m_oilPressure;
}
int store::getOilTemperature() const{
	return this->m_oilTemperature;
}
float store::getBatteryVoltage() const{
	return this->m_batteryVoltage;
}
int store::getVehicleSpeed() const{
	return this->m_vehicleVelocity;
}
int store::getDataLoggerStatus() const{
	return this->m_dataLoggerStatus;
}
float store::getLambda() const{
	return this->m_lambdaMixtureAirFuel;
}
int store::getTcSlip() const{
	return this->m_tractionSlip;

}
int store::getTcLaunch() const{
	return this->m_tractionLaunch;
}
void store::setRpm(int rpm){
    if(rpm>=0&& rpm !=this->m_rotationsPerMinute){
		int oldRpm=this->m_rotationsPerMinute;
		this->m_rotationsPerMinute=rpm;
		emit rpmChanged(this->m_rotationsPerMinute, oldRpm);
	}
}
void store::setGearShift(int gearShift){
    if(gearShift>=0){
		this->m_gearShift=gearShift;
	}
}
void store::setEngineTemperature(int engineTemperature){
	this->m_engineTemperature=engineTemperature;

}
void store::setOilPressure(float oilPressure){
	this->m_oilPressure=oilPressure;

}
void store::setOilTemperature(int oilTemperature){
	this->m_oilTemperature=oilTemperature;
}
void store::setBatteryVoltage(float batteryVoltage){
	this->m_batteryVoltage=batteryVoltage;
}
void store::setVehicleSpeed(int vehicleVelocity){
	this->m_vehicleVelocity=vehicleVelocity;
}
void store::setDataLoggerStatus(int dataLoggerStatus){
	this->m_dataLoggerStatus=dataLoggerStatus;
}
void store::setLambda(float lambda){
	this->m_lambdaMixtureAirFuel=lambda;
}
void store::setTcSlip(int tcSlip){
	this->m_tractionSlip=tcSlip;
}
void store::setTcLaunch(int tcLaunch){
	this->m_tractionLaunch=tcLaunch;
}
