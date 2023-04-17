#include "store.h"
#include "references/bson_var.h"

using json = nlohmann::json;
int store::setupSerial() {
	
	QSerialPort* serial= new QSerialPort();
	serial->setPortName(this->dev);
	serial->setBaudRate(this->baud);
	serial->setDataBits(QSerialPort::Data8);
	serial->setStopBits(QSerialPort::OneStop);
	serial->setParity(QSerialPort::NoParity);
	serial->setFlowControl(QSerialPort::NoFlowControl);
	if (!serial->open(QIODevice::ReadWrite)) {
		qDebug() << "Can't open " << this->dev << ", error code" << serial->error();
		serialLog.append("||Can't open " + this->dev + ", error code" + serial->error()+"||");
		return 1;
	}

	this->port = serial;
	connect(this->port, &QSerialPort::readyRead, this, &store::handleReadyRead);
	connect(this->port, &QSerialPort::errorOccurred, this, &store::handleError);
	

	return 0;	
}
//MUST REVIEW!! this probs is overcomplicated and im too tired+(my code effect)
int store::startGeneralErrorLog(uint depth){
    //check if file is not null
    if(!errorLog.exists()){
		//get todays date and time and use it as a filename
		QDateTime now = QDateTime::currentDateTime();
        QString dateStr = now.toString("hhmmss_dd-MM-yyyy");
        dateStr = "errorLog_"+dateStr+".log";
        errorLog.setFileName(dateStr);

        if(errorLog.open(QIODevice::WriteOnly|QIODevice::Unbuffered)){
            return 1;
        }
        return 0;
    }
	try {
        if(!errorLog.isOpen()){
			scribeError("Apparently the error log was closed, yet the pointer was not set to nullptr.", error_severity::MAJOR);
			if(depth>0){
				//... Qcreator kept saying me this was possible...
				scribeError("Something is really wrong, the depth is too big, and memory seems insane", error_severity::CRITICAL);
			}
			return startGeneralErrorLog(++depth);
		}else{
			scribeError("A request to open error log was made yet the file was already opened, Avoid multiple calls to startGeneralErrorLog", error_severity::WARNING);
		
		}
	} catch (...) {
		//TODO handle exception graphically
		qDebug() << "An exception occurred while trying to open the error log";
		return 1; //TODO return a more meaningful error code
	}
}

void store::stopGeneralErrorLog(){
    try {
        if(!errorLog.isOpen()){
            throw "The error log was already closed";
			
        }
        errorLog.close();
    } catch (...) {
		//TODO handle exception graphically
		qDebug() << "An exception occurred while trying to close the error log";
    }
}
qint64 store::scribeError(QString error, error_severity severity){
	qint64 ret = 0;
	//check if error is initialized if not
	if(error==nullptr){
		error = "An error occurred but no error message was provided";
	}

	try{		
        if(!errorLog.isOpen()){
			startGeneralErrorLog();
		}

		//append a timestamp to the error and write it to the file
		QDateTime now = QDateTime::currentDateTime();
		QString dateStr = now.toString("hh:mm:ss dd-MM-yyyy");
		error.prepend(dateStr + " |" + QString::number(severity) + "|*_");

        ret= errorLog.write(error.toUtf8()+"|EOL|\n");
		if (severity>=error_severity::CRITICAL){
			//TODO handle exception graphically
			
			//freeze the program for 3 secs
            this->thread()->msleep(3000);
			
			//its pretty much garanteed but just in case
			assert(severity>=error_severity::CRITICAL);
		}
	}catch(...){
		//TODO handle exception graphically
		qDebug() << "An exception occurred while trying to write to the error log";
	}

	return ret;
}
store::store( QString dev, QSerialPort::BaudRate baud, QObject *parent): QObject(parent){
	this->baud = baud;
	//if Qstring dev is empty, use default device
	if(dev.isEmpty()){
		this->dev = DEFAULT_DEVICE;
	}else{
		this->dev = dev;
	}
	setupSerial();
    //int8_t retries = LOG_MAX_RETRIES;
    //wtf? TODO: retry system
    startGeneralErrorLog();
	
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

    bufferMessage=port->readAll();
    serialLog.append(bufferMessage);
	//can be optimized using pointers or even a variable as a "bookmark" wether a int or pointer 
	lastMessage.append(bufferMessage);

	//regex BSON_WARNING value in bufferMessage if its found set markerBSON_WARNING to the first character after BSON_WARNING

    if(lastMessage.contains(BSON_WARNING)){
		bsonMining();
    }
	

}
void store::bsonMining(){
	//lastMessage starts at lastMessage size - ((lenght of buffer - index of BSON_WARNING) + length of BSON_WARNING)
	const int bson_len = strlen(BSON_WARNING);
	//im assuming a minimum of 9 due to warning=4, document size = 4, EOF =1, sum of them all is 9
    if(lastMessage.size()<=BSON_SKIP_BYTES || lastMessage.indexOf(BSON_WARNING)==-1){

		return;
	}
	int marcador = lastMessage.indexOf(BSON_WARNING) + bson_len;
	auto shrinked=lastMessage.mid(marcador);
	//get the first 4 bytes of the lastMessage and convert to int
	
	int length = shrinked[0] | shrinked[1] << 8| shrinked[2] <<16| shrinked[3]<<24;
	if(shrinked.size() < length){
		//qDebug() << "BSON WARNING FOUND BUT NOT ENOUGH BYTES";
		return;
	}
	
	shrinked = shrinked.mid(0, length);

	std::vector<std::uint8_t> v(shrinked.begin(),shrinked.end());

	parseBson(v);
	lastMessage=lastMessage.mid(marcador+length);
	return bsonMining();
}
store::~store(){
	//make shure all slots are disconnected
	
	this->disconnect();
	try{
		closeSerial();

		QFile file("serialLog.txt");
		if (file.open(QIODevice::WriteOnly)) {
			file.write(serialLog);
			file.close();
		}else{
			scribeError("Failed to write serial log to file", error_severity::MAJOR);
		}

		stopGeneralErrorLog();
	}catch(...){
		qDebug() << "Failed to destroy store";
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
void store::parseBson(std::vector<std::uint8_t> v){
	try {
        

        json j = json::from_bson(v);
		//TODO this is ugly 
        if(j.contains(BSON_RPM)){
			this->setRpm(j[BSON_RPM]);
		}
		if(j.contains(BSON_GEARSHIFT)){
			this->setGearShift(j[BSON_GEARSHIFT]);
		}
		if(j.contains(BSON_ENGINETEMPERATURE)){
			this->setEngineTemperature(j[BSON_ENGINETEMPERATURE]);
		}

	} catch (json::parse_error& e) {
        qDebug() << "parse error at byte " << e.byte << "\n";
        qDebug() << "message: " << v << "\n";
        qDebug() << v.size();
        //lastMessage.clear();


	}
    

}

int store::closeSerial(){
	// TODO: Error handling?
    try {
        if(port->isOpen()){
            port->close();
        }
     }catch(...){
            qDebug()<< "Couldnt close the serial port";
            return 1;

    }
    return 0;
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
    if(rpm>=0){
		if(rpm !=this->m_rotationsPerMinute){
			int oldRpm=this->m_rotationsPerMinute;
			this->m_rotationsPerMinute=rpm;
			emit rpmChanged(this->m_rotationsPerMinute, oldRpm);
		}
    }else{
        this->scribeError(__FSIPLEIRIA_STORE_SETRPM_ERROR__, store::error_severity::MINOR);
    }
}
void store::setGearShift(int gearShift){
    if(gearShift>=0&&gearShift<=6){
		if(gearShift !=this->m_gearShift){
			int oldGearShift=this->m_gearShift;
			this->m_gearShift=gearShift;
			emit gearShiftChanged(this->m_gearShift, oldGearShift);
		}
	}else{
		this->scribeError(__FSIPLEIRIA_STORE_SETGEARSHIFT_ERROR__, store::error_severity::MINOR);
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
void store::setBaudRate(QSerialPort::BaudRate baud){
	this->baud=baud;
	this->port->setBaudRate(baud);
	
}
QSerialPort::BaudRate store::getBaudRate() const{
	return baud;
}
