#include "store.h"
#include <cstdint>
#include <QFile>
using json = nlohmann::json;

int store::setupSerial() {
	
    QSerialPort* serial= new QSerialPort();
    serial->setPortName(this->dev);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (!serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Can't open " << this->dev << ", error code" << serial->error();
        return 1;
    }

    this->port = serial;
    connect(this->port, &QSerialPort::readyRead, this, &store::handleReadyRead);
    connect(this->port, &QSerialPort::errorOccurred, this, &store::handleError);
	

	return 0;	
}

store::store( QString dev, QObject *parent  ): QObject(parent){
	//if Qstring dev is empty, use default device
	if(dev.isEmpty()){
		this->dev = DEFAULT_DEVICE;
	}else{
		this->dev = dev;
	}

	setupSerial();
	
}
void store::forceRead(qint64 len){
	while (port->bytesAvailable() < len) {
		if (!port->waitForReadyRead(1000)) {
			qDebug() << QObject::tr("Wait read error %1")
						.arg(port->errorString());
			return;
		}
		
	}
    port->read(bufferMessage.data(), len);
}
//destructor for store 

void store::handleReadyRead(){
    qDebug() << "FIRED";
	
    bufferMessage=port->readAll();
	serialLog.append(bufferMessage);
    //disconnect(this->port, &QSerialPort::readyRead, this, &store::handleReadyRead);

	//can be optimized using pointers or even a variable as a "bookmark" wether a int or pointer 
	lastMessage.append(bufferMessage);
    /*uint32_t size = (int)lastMessage[0] | (int)lastMessage[1] << 8 | (int)lastMessage[2] << 16 | (int)lastMessage[3] << 24;
	int8_t eof = 0x00;
	
    if((bool)((long unsigned int)lastMessage.size() == size+sizeof(size)+sizeof(eof))&& ((bool) lastMessage[lastMessage.size()-1] == eof)){
		parseJson();
		//clear lastMessage()
		lastMessage.clear();
    }*/
	
	

}
store::~store(){
	//print to a file serial log
	//close serial port
    QFile file("serialLog.txt");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(serialLog);
        file.close();
    }

	closeSerial();
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
void store::parseJson(){
    json j = json::from_bson(lastMessage);
	//read element "rpm"
	if(j.contains("rpm")){
		this->setRpm(j["rpm"]);
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
