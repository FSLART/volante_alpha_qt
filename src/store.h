#ifndef STORE_H
#define STORE_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <thread>
#include <cstdio>
#include <sys/types.h>
#include <vector>
#include <QCoreApplication>
#include <QErrorMessage>
#include <QFile>
#include <QObject>

#include <QDebug>
#include <QSerialPort>
#include <QDateTime>
#include <QThread>
//QVBoxLayout
#include <QVBoxLayout>
#include <nlohmann/json.hpp>
#include "references/bson_var.h"
#if !defined __arm__ || !defined __aarch64__
    #ifdef _WIN32
		#define DEFAULT_DEVICE "COM3"
    #elif defined __linux__
		#ifdef __FSIPLEIRIA_DEPLOY__
			#define DEFAULT_DEVICE "/dev/ttyS0"
		#else
            #define DEFAULT_DEVICE "/dev/ttyUSB0"
		#endif
	#endif
#else
    #define DEFAULT_DEVICE "/dev/ttyS0"
#endif

#define BSON_WARNING "\xFF\xFF\xFF\xFF"
#define LOG_MAX_RETRIES 4
#define BSON_SKIP_BYTES 9
/* !
        \class store
        \brief Basicamente uma store contendo as vários dados necessários a serem transmitidos ou representados graficamente

		Comecemos com o resumo pois se estás a ler isto provavelmente não e de completa livre vontade,
		esta classe representa um ponto centralizado dos dados semelhante a ideologia que e muito usada em sistemas distribuídos(especialmente falando em JS, Vue, React, etc...)
		
		\fields
*/

class store: public QObject{
    Q_OBJECT;
    Q_PROPERTY(int  m_rotationsPerMinute READ getRpm WRITE setRpm NOTIFY rpmChanged);



	public:
        enum error_severity {
            INFO=0,
            WARNING=1,
            MINOR=2,
            MAJOR=3,
            CRITICAL=100
        };
        QString dev;
		QSerialPort* port=nullptr;
		void handleReadyRead();
		void forceRead(qint64 len);
		void handleError(QSerialPort::SerialPortError serialPortError);
		
		QByteArray serialLog;
		QByteArray lastMessage;
		QByteArray bufferMessage;
		char * markerBSON_WARNING=nullptr; 
		
		void parseBson(std::vector<std::uint8_t> v);
                void bsonMining();
                int requestSlotAttachment();
        qint64 scribeError(QString error, error_severity severity=error_severity::INFO);
        explicit store(QString dev="", QSerialPort::BaudRate baud = QSerialPort::Baud38400, QObject *parent = nullptr);
		~store();
		
		//getters and setters
		QSerialPort::BaudRate getBaudRate() const;

		int getRpm() const;
		int getGearShift() const;
		int getEngineTemperature() const;
		float getOilPressure() const;
		int getOilTemperature() const;
		float getBatteryVoltage() const;
		int getVehicleSpeed() const;
		int getDataLoggerStatus() const;
		float getLambda() const;
		int getTcSlip() const;
		int getTcLaunch() const;
		void setRpm(int rpm);
		void setGearShift(int gearShift);
		void setEngineTemperature(int engineTemperature);
		void setOilPressure(float oilPressure);
		void setOilTemperature(int oilTemperature);
		void setBatteryVoltage(float batteryVoltage);
		void setVehicleSpeed(int vehicleVelocity);
		void setDataLoggerStatus(int dataLoggerStatus);
		void setLambda(float lambda);
		void setTcSlip(int tcSlip);
		void setTcLaunch(int tcLaunch);

		void setBaudRate(QSerialPort::BaudRate baud);

	protected:
        int startGeneralErrorLog(uint depth=0);
		void stopGeneralErrorLog();
                int setupSerial();
        //        bool setSlots=false;
        //        int setupSlots();
		int closeSerial();

	signals:
		void rpmChanged(int newRpm, int oldRpm);
		void gearShiftChanged(int newGearShift, int oldGearShift);
		void engineTemperatureChanged(int newEngineTemperature, int oldEngineTemperature); 
		void oilTemperatureChanged(int newOilTemperature, int oldOilTemperature); 
		void oilPressureChanged(int newOilPressure, int oldOilPressure); 
		void batteryVoltageChanged(int newBatteryVoltage, int oldBatteryVoltage); 
		void vehicleSpeedChanged(int newVehicleSpeed, int oldVehicleSpeed); 
		void dataLoggerChanged(int newDataLogger, int oldDataLogger); 
		void lambdaChanged(int newLambda, int oldLambda); 
		void tcSlipChanged(int newTCSlip, int oldTCSlip); 
		void tcLaunchChanged(int newTcLaunch, int oldTcLaunch);  


    private:
		QSerialPort::BaudRate baud;
        QFile errorLog;
        int m_rotationsPerMinute=0;
        int m_gearShift=0;
        int m_engineTemperature=0;
        float m_oilPressure=0;
        float m_oilTemperature=0;
        float m_batteryVoltage=0;
        int m_vehicleVelocity=0;
        int m_dataLoggerStatus=0;
        float m_lambdaMixtureAirFuel=0;
        int m_tractionSlip=0;
        int m_tractionLaunch=0;

};

// Logging Macros
#define __FSIPLEIRIA_STORE_SETRPM_ERROR__ "store::setRpm(int rpm)->Rpm is negative"
#define __FSIPLEIRIA_STORE_SETGEARSHIFT_ERROR__ "store::setGearShift(int gearShift)->GearShift is out of bounds"

#endif // STORE_H
