/**
* @file store.cpp
* @brief This file contains the logic for the store class
* @see store.h
* @see mainwindow.h
* @see flabel.h
* @author João Vieira
* This piece of software was developed for the T24e project belonging to the LART Team
**/
#include "store.h"
#include "mainwindow.h"
#include "references/bson_var.h"
#include "flabel.h"


using json = nlohmann::json;
/**
* @brief Arguably the boilerplate code, necessary to run an Asynchronous Serial Uart communication. Also responsible for handling @b slots and other callback logic
* @see https://doc.qt.io/qt-5/qtserialport-creaderasync-example.html
*      wow the above was suggested by copilot. how many times did people have issues with this?
**/
int store::setupSerial() {
	
	QSerialPort* serial= new QSerialPort();
	serial->setPortName(this->dev);
	serial->setBaudRate(this->baud);
	serial->setDataBits(QSerialPort::Data8);
	serial->setStopBits(QSerialPort::OneStop);
	//serial->setParity(QSerialPort::OddParity);
	serial->setFlowControl(QSerialPort::NoFlowControl);
	if (!serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Can't open " << this->dev << ", error code" << serial->error();
        serialLog.append("||Can't open " + this->dev + ", error code" + serial->errorString()+"||");
		return 1;
	}

        this->port = serial;
        //clear garbage
        this->port->flush();
	connect(this->port, &QSerialPort::readyRead, this, &store::handleReadyRead);
	connect(this->port, &QSerialPort::errorOccurred, this, &store::handleError);
	

	return 0;	
}
/**
* @brief Starts the error logging system / journal, if it doesnt exist, creates it.
* @param depth The depth of the recursion, used to prevent infinite recursion
* @return 0 if success, 1 if failure
**/
//TODO MUST REVIEW!! this probs is overcomplicated and im too tired+(my code effect) (side note: this still wasnt reviewed. Dont complain about issues here...); 
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
    return 0;
}
/**
* @brief Stops the error logging system / journal, if it exists, closes it.
* @return 0 if success, 1 if failure

**/
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
/**
* @brief Writes an error to the error log
* @param error The error message
* @param severity The severity of the error
* @return The number of bytes written to the file
* @see store.h
**/
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
/**
* @brief Constructor of the store class, responsible for setting up communications and the error log
* @param dev The device to be used, defaults to DEFAULT_DEVICE
* @param baud The baud rate to be used, defaults to 115200
* @param parent The parent of the store class, defaults to nullptr
* @see store.h
**/
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
    //TODO wtf? retry system
    startGeneralErrorLog();
}
/**
* @brief Forces the serial port to read a certain amount of bytes. 
*		 @b This is a blocking function and isnt being used as of the moment. Its mostly for profiling environment and testing.
* @param len The amount of bytes to be read
* @see store.h
**/
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
/**
* @brief Slot callback for the serial port, handles the reading of the serial port and indirectly the parsing of the BSON data. 
*		 @b NON-BLOCKING function, you should'nt call this function directly.
*		 @b IMPURE function, obviously...
* @see store.h
**/
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
/**
* @brief function mostly used to prevent spaghetti code in the handleReadyRead function. Contains the logic required to parse the bson message
*		 @b IMPURE function.
**/
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
		//TODO handle exception graphically
		//qDebug() << "BSON WARNING FOUND BUT NOT ENOUGH BYTES";
		return;
	}
	
	shrinked = shrinked.mid(0, length);

	std::vector<std::uint8_t> v(shrinked.begin(),shrinked.end());

	parseBson(v);
	lastMessage=lastMessage.mid(marcador+length);
	return bsonMining();
}
/**
* @brief Destructor of the store class, responsible for closing the serial port and writing the serial log to a file
**/
store::~store(){
	//make shure all slots are disconnected
	
	this->disconnect();
	try{
		closeSerial();
		QDateTime now = QDateTime::currentDateTime();
                QString dateStr = now.toString("hhmmss_dd-MM-yyyy");
                dateStr.append("serialLog.bin");
                QFile file(dateStr);
		if (file.open(QIODevice::WriteOnly)) {
			file.write(serialLog);
			file.close();
		}else{
			//TODO figure out if this is able to handle out of disk exceptions. Not that it will ever reach it but still...
			scribeError("Failed to write serial log to file", error_severity::MAJOR);
		}

		stopGeneralErrorLog();
	}catch(...){
		qDebug() << "Failed to destroy store";
	}
	
}
/**
* @brief Slot callback for the serial port, handles the errors of the serial port.
*		 @b  NON-BLOCKING function, you should'nt call this function directly.
* @param serialPortError The error code of the serial port
* @see store.h

**/
void store::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        scribeError("An I/O error occurred while reading the data from port " + this->dev + " error code: " + QString::number(serialPortError), error_severity::CRITICAL);
        QCoreApplication::exit(1);
    }
}
//TODO maybe do this but non blocking? 
/**
* @brief Parses the BSON data and updates the variables of the store class
*	     @b Blocking function
* @param v The BSON data
* @see store.h
**/
void store::parseBson(std::vector<std::uint8_t> v){
	try {
        json j = json::from_bson(v);
		//TODO this is ugly 
		
        if(j.contains(BSON_RPM)){
			this->setRpm(j[BSON_RPM]);
		}

		if(j.contains(BSON_ENGINETEMPERATURE)){
			this->setEngineTemperature(j[BSON_ENGINETEMPERATURE]);
		}
		
		if(j.contains(BSON_BATTERYVOLTAGE)){
			EncodingUnion t;
			t.encoded=j[BSON_BATTERYVOLTAGE]; 
			this->setBatteryVoltage(t.decoded);
		}
		if(j.contains(BSON_VEHICLESPEED)){
			this->setVehicleSpeed(j[BSON_VEHICLESPEED]);
		}
		// SPAGGETI Macro for __the win__ Darn you co pilot what a cringe move, 
		// anyway i was saying that this handles portability between vehicles.
		#ifdef __LART_T14__
			if(j.contains(BSON_AFR)){
				EncodingUnion t;
				t.encoded=j[BSON_AFR];
				this->setLambda(t.decoded);
			}
			if(j.contains(BSON_TCSLIP)){
				this->setTcSlip(j[BSON_TCSLIP]);
			}
			if(j.contains(BSON_TCLAUNCH)){
				this->setTcLaunch(j[BSON_TCLAUNCH]);
			}
			if(j.contains(BSON_GEARSHIFT)){
				this->setGearShift(j[BSON_GEARSHIFT]);
			}
			if(j.contains(BSON_DATALOGGERSTATUS)){
				this->setDataLoggerStatus(j[BSON_DATALOGGERSTATUS]);
			}
			if(j.contains(BSON_OILPRESSURE)){
				EncodingUnion t;
				t.encoded=j[BSON_OILPRESSURE];
				this->setOilPressure(t.decoded);
			}
			if(j.contains(BSON_OILTEMPERATURE)){
				EncodingUnion t;
				t.encoded=j[BSON_OILTEMPERATURE]; 
				this->setOilTemperature(t.decoded);
			}
		#endif
		#ifdef __LART_T24__
			if(j.contains(BSON_SOC)){
				EncodingUnion t;
				t.encoded=j[BSON_SOC];
				this->setSoc(t.decoded);
			}
			
			if(j.contains(BSON_BATTERYTEMPERATURE)){
				EncodingUnion t;
				t.encoded=j[BSON_BATTERYTEMPERATURE];
				this->setBatteryTemperature(t.decoded);
			}
			if(j.contains(BSON_INVERTERTEMPERATURE)){
				this->setInverterTemperature(j[BSON_INVERTERTEMPERATURE]);
			}
                         if(j.contains(BSON_POWER)){
                                short temp =  j[BSON_POWER];
                                this->setPower(temp);
			}
			if(j.contains(BSON_LAPTIME)){
				this->setLapTime(j[BSON_LAPTIME]);
			}
                        if(j.contains(BSON_LAPCOUNT)){
                                            short temp = j[BSON_LAPCOUNT];
                            this->setLapCount(temp);

			}
                        if(j.contains(BSON_HV)){
                                short temp = j[BSON_HV];
                            this->setHV(temp); ;

                        }
			//if(j.contains(BSON_TYRETEMPERATURE)){
			//	this->setTyreTemperature(j[BSON_TYRETEMPERATURE]);
			//}


		#endif
	
	} catch (json::parse_error& e) {
		scribeError("An error occurred while parsing the BSON data, error: " + QString::fromStdString(e.what()), error_severity::MINOR);
        //TODO study if bellow is better or worse.
		//lastMessage.clear();


	}
    

}
/**
* @brief Closes the serial port
* @return 0 if success, 1 if failure
**/
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
/**
* @brief getter for the rpm variable
* @return The rpm variable
**/
int store::getRpm() const{
	return this->m_rotationsPerMinute;
}
/**
* @brief setter for the rpm variable
* @param rpm The new value for the rpm variable
**/
void store::setRpm(int rpm){
    if(rpm>=0){
		if(rpm !=this->m_rotationsPerMinute){
			int oldRpm=this->m_rotationsPerMinute;
			this->m_rotationsPerMinute=rpm;
			emit rpmChanged(this->m_rotationsPerMinute, oldRpm);
		}
    }else{
        this->scribeError(__LART_STORE_SETRPM_ERROR__, store::error_severity::MINOR);
    }
}
/**
* @brief getter for the engine temperature variable
* @return The engine temperature variable
**/
int store::getEngineTemperature() const{
	return this->m_engineTemperature;
}
/**
* @brief setter for the engine temperature variable
* @param engineTemperature The new value for the engine temperature variable
**/
void store::setEngineTemperature(int engineTemperature){
	qDebug() << "setEngineTemperature";
	int oldEngineTemperature = this->m_engineTemperature;
	this->m_engineTemperature=engineTemperature;
	emit engineTemperatureChanged(this->m_engineTemperature, oldEngineTemperature);

}
/**
* @brief getter for the battery voltage variable
* @return The battery voltage variable
**/
float store::getBatteryVoltage() const{
	return this->m_batteryVoltage;
}
/**
* @brief setter for the battery voltage variable
* @param batteryVoltage The new value for the battery voltage variable
**/
void store::setBatteryVoltage(float batteryVoltage){
	float oldBatteryVoltage = this->m_batteryVoltage;
	this->m_batteryVoltage=batteryVoltage;
	emit batteryVoltageChanged(this->m_batteryVoltage, oldBatteryVoltage);
}
/**
* @brief getter for the vehicle speed variable
* @return The vehicle speed variable
**/
int store::getVehicleSpeed() const{
	return this->m_vehicleVelocity;
}
/**
* @brief setter for the vehicle speed variable
* @param vehicleVelocity The new value for the vehicle speed variable
**/
void store::setVehicleSpeed(int vehicleVelocity){
	int oldVehicleVelocity = this->m_vehicleVelocity;
	this->m_vehicleVelocity=vehicleVelocity;
	emit vehicleSpeedChanged(this->m_vehicleVelocity, oldVehicleVelocity);
}



#ifdef __LART_T14__
/**
* @brief getter for the data logger status
* @return The data logger status variable
**/
int store::getDataLoggerStatus() const{
	return this->m_dataLoggerStatus;
}
/**
* @brief getter for the lambda variable
* @return The lambda variable
**/
float store::getLambda() const{
	return this->m_lambdaMixtureAirFuel;
}
/**
* @brief getter for the traction slip variable
* @return The traction slip variable
**/
int store::getTcSlip() const{
	return this->m_tractionSlip;

}
/**
* @brief getter for the traction launch variable
* @return The traction launch variable
**/
int store::getTcLaunch() const{
	return this->m_tractionLaunch;
}
/**
* @brief getter for the gear shift variable
* @return The gear shift variable
**/
int store::getGearShift() const{
	return this->m_gearShift;
}
/**
* @brief setter for the gear shift variable
* @param gearShift The new value for the gear shift variable
**/
void store::setGearShift(int gearShift){
	qDebug() << "setGearShift";
    if(gearShift>=0&&gearShift<=6){
		if(gearShift !=this->m_gearShift){
			int oldGearShift=this->m_gearShift;
			this->m_gearShift=gearShift;
			emit gearShiftChanged(this->m_gearShift, oldGearShift);
		}
	}else{
		this->scribeError(__LART_STORE_SETGEARSHIFT_ERROR__, store::error_severity::MINOR);
	}
}
/**
* @brief setter for the data logger status variable
* @param dataLoggerStatus The new value for the data logger status variable
**/
float store::getOilPressure() const{
	return this->m_oilPressure;
}
/**
* @brief getter for the oil pressure variable
* @return The oil pressure variable
**/
float store::getOilTemperature() const{
	return this->m_oilTemperature;
}
/**
* @brief setter for the oil temperature variable
* @param oilTemperature The new value for the oil temperature variable
**/
void store::setOilPressure(float oilPressure){
    float oldOilPressure = this->m_oilPressure;
	this->m_oilPressure=oilPressure;
	emit oilPressureChanged(this->m_oilPressure, oldOilPressure);

}
/**
* @brief setter for the oil temperature variable
* @param oilTemperature The new value for the oil temperature variable
**/
void store::setOilTemperature(float oilTemperature){
        float oldOilTemperature = this->m_oilTemperature;
	this->m_oilTemperature=oilTemperature;
	emit oilTemperatureChanged(this->m_oilTemperature, oldOilTemperature);
}
/**
* @brief setter for the data logger status variable
* @param dataLoggerStatus The new value for the data logger status variable
**/
void store::setDataLoggerStatus(int dataLoggerStatus){
	int oldDataLoggerStatus = this->m_dataLoggerStatus;
	this->m_dataLoggerStatus=dataLoggerStatus;
	emit dataLoggerChanged(this->m_dataLoggerStatus, oldDataLoggerStatus);
}
/**
* @brief setter for the lambda variable
* @param lambda The new value for the lambda variable
**/
void store::setLambda(float lambda){
        float oldLambda = this->m_lambdaMixtureAirFuel;
	this->m_lambdaMixtureAirFuel=lambda;
	emit lambdaChanged(this->m_lambdaMixtureAirFuel, oldLambda);

}
/**
* @brief setter for the traction slip variable
* @param tcSlip The new value for the traction slip variable
**/
void store::setTcSlip(int tcSlip){
	int oldTcSlip = this->m_tractionSlip;
	this->m_tractionSlip=tcSlip;
	emit tcSlipChanged(this->m_tractionSlip, oldTcSlip);
}
/**
* @brief setter for the traction launch variable
* @param tcLaunch The new value for the traction launch variable
**/
void store::setTcLaunch(int tcLaunch){
	int oldTcLaunch = this->m_tractionLaunch;
	this->m_tractionLaunch=tcLaunch;
	emit tcLaunchChanged(this->m_tractionLaunch, oldTcLaunch);
}

#endif
/**
* @b Start of T24
**/
#ifdef __LART_T24__
/**
* @brief getter for the state of charge variable
* @return The state of charge variable
**/
float store::getSoc() const{
	return this->m_stateOfCharge;
}
/**
* @brief getter for the battery temperature variable
* @return The battery temperature variable
**/
float store::getBatteryTemperature() const{
	return this->m_batteryTemperature;
}
/**
* @brief getter for the inverter temperature variable
* @return The inverter temperature variable
**/
int store::getInverterTemperature() const{
	return this->m_inverterTemperature;
}
/**
* @brief getter for the power variable
* @return The power variable
**/
short store::getPower() const{
	return this->m_power;
}
/**
* @brief getter for the lap time  @b Double-Word Integer variable,
* @return The lap time variable
**/
int store::getLapTime() const{
	return this->m_lapTime;
}
/**
* @brief getter for the lap count variable
* @return The lap count variable
**/
short store::getLapCount() const{
	return this->m_lapCount;
}
/**
* @brief getter for the high voltage variable
* @return The high voltage variable
 */
short store::getHV() const{
	return this->m_highVoltage;
}

/**
* @brief getter for the tyre temperature variable
* @return The tyre temperature variable
**/
//int store::getTyreTemperature() const{
//	return this->m_tyreTemperature;
//}
/**
* @brief setter for the state of charge variable
* @param soc The new value for the state of charge variable
**/
void store::setSoc(float soc){
	float oldSoc = this->m_stateOfCharge;
	this->m_stateOfCharge=soc;
	emit socChanged(this->m_stateOfCharge, oldSoc);
}
/**
* @brief setter for the battery temperature variable
* @param batteryTemperature The new value for the battery temperature variable
**/
void store::setBatteryTemperature(float batteryTemperature){
	float oldBatteryTemperature = this->m_batteryTemperature;
	this->m_batteryTemperature=batteryTemperature;
	emit batteryTemperatureChanged(this->m_batteryTemperature, oldBatteryTemperature);
}
/**
* @brief setter for the inverter temperature variable
* @param inverterTemperature The new value for the inverter temperature variable
**/
void store::setInverterTemperature(int inverterTemperature){
	int oldInverterTemperature = this->m_inverterTemperature;
	this->m_inverterTemperature=inverterTemperature;
	emit inverterTemperatureChanged(this->m_inverterTemperature, oldInverterTemperature);
}
/**
* @brief setter for the power variable
* @param power The new value for the power variable
**/
void store::setPower(short power){
	short oldPower = this->m_power;
	this->m_power=power;
	emit powerChanged(this->m_power, oldPower);
}
/**
* @brief setter for the lap time @b Double-Worded Integer variable. <b>Also Handles Lap Differences</b>
* @param lapTime The new value for the lap time variable
**/
void store::setLapTime(int lapTime){
	int oldLapTime = this->m_lapTime;
	this->m_lapTime=lapTime;
	//transform lapTime int into QTime 
	//TODO document this
	//CurrentLapTimes
	QTime time = QTime::fromMSecsSinceStartOfDay(this->m_lapTime);
	QTime oldtime = QTime::fromMSecsSinceStartOfDay(oldLapTime);

	//DiffLapTimes
	QTime diffTime = QTime::fromMSecsSinceStartOfDay(this->m_lapTime - oldLapTime);
	//Pluggin this is not used for anything
	QTime oldDiffTime = QTime::fromMSecsSinceStartOfDay(0);

	

	emit diffLapTimeChanged(diffTime, oldDiffTime);
	emit lapTimeChanged(time, oldtime);
}
/**
* @brief setter for the lap count variable
* @param lapCount The new value for the lap count variable
**/
void store::setLapCount(short lapCount){
	short oldLapCount = this->m_lapCount;
	this->m_lapCount=lapCount;
	emit lapCountChanged(this->m_lapCount, oldLapCount);
}
/**
* @brief setter for the high voltage variable
* @param hv The new value for the high voltage variable
**/
void store::setHV(short hv){
	short oldHV = this->m_highVoltage;
	this->m_highVoltage=hv;
	emit hvChanged(this->m_highVoltage, oldHV);
}

//void store::setTyreTemperature(int tyreTemperature){
//	int oldTyreTemperature = this->m_tyreTemperature;
//	this->m_tyreTemperature=tyreTemperature;
//	emit tyreTemperatureChanged(this->m_tyreTemperature, oldTyreTemperature);
//}
#endif
/**
* @brief setter for the baud rate variable
* @param baud The new value for the baud rate variable
**/
void store::setBaudRate(QSerialPort::BaudRate baud){

	this->baud=baud;
	this->port->setBaudRate(baud);
	
}
/**
* @brief getter for the baud rate variable
* @return QSerialPort::BaudRate The baud rate variable
*/
QSerialPort::BaudRate store::getBaudRate() const{
	return baud;
}
