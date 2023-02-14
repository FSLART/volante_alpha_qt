#include "store.h"
#include <cstdint>
using json = nlohmann::json;

int store::setupSerial() {
	
    QSerialPort* serial= new QSerialPort();
    serial->setPortName(this->dev);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (!serial->open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open " << this->dev << ", error code" << serial->error();
        return 1;
    }

    this->port = serial;
    connect(this->port, &QSerialPort::readyRead, this, &store::handleReadyRead);
    connect(this->port, &QSerialPort::errorOccurred, this, &store::handleError);
	

	return 0;	
}

store::store( char * dev, QObject *parent  ): QObject(parent){
    if (dev == nullptr){
        // TODO: fix this(use a better function preferably one handled by QT)
        int len = sizeof(char)*strlen(DEFAULT_DEVICE)+1;
        this->dev = (char*)malloc(len);
        strcpy(this->dev,DEFAULT_DEVICE);
	}
	//copy dev to this->dev
	else{
		int len = sizeof(char)*strlen(dev)+1;
		this->dev = (char*)malloc(len);
		strcpy(this->dev,dev);
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
	bufferMessage=port->readAll(); 
	serialLog.append(bufferMessage);

	//can be optimized using pointers or even a variable as a "bookmark" wether a int or pointer 
	lastMessage.append(bufferMessage);
	uint32_t size = (int)lastMessage[0] | (int)lastMessage[1] << 8 | (int)lastMessage[2] << 16 | (int)lastMessage[3] << 24;
	int8_t eof = 0x00;
	
    if((bool)((long unsigned int)lastMessage.size() == size+sizeof(size)+sizeof(eof))&& ((bool) lastMessage[lastMessage.size()-1] == eof)){
		parseJson();
		//clear lastMessage()
		lastMessage.clear();
	}
	
	

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
	std::cout << std::setw(2) << j << std::endl;

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
