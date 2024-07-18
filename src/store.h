/**
* @file store.h
* @brief This file contains the declaration of the store class
* @see store.cpp
* @see mainwindow.h
* @see flabel.h
* @author João Vieira
* This piece of software was developed for the T24e project belonging to the LART Team
**/
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
/**
* @brief Go big or go home, :) 
**/
#if !defined __arm__ || !defined __aarch64__
    #ifdef _WIN32
		#define DEFAULT_DEVICE "COM3"
    #elif defined __linux__
		#ifdef __LART_DEPLOY__
			#define DEFAULT_DEVICE "/dev/ttyS3"
		#else
            #define DEFAULT_DEVICE "/dev/ttyUSB0"
		#endif
	#endif
#else
    #define DEFAULT_DEVICE "/dev/ttyS3"
#endif
// Mostly used for "de-typing" data.
typedef union {
	float decoded;
	int32_t encoded;
} EncodingUnion;

// Header of start of Bson Message, this is used to identify the start of a message, however it is not used for parsing. and in the future a better way of doing this is possible. Trust the standard!
#define BSON_WARNING "\xFF\xFF\xFF\xFF"
#define LOG_MAX_RETRIES 4
// this is a constant pretty much dont touch it..
#define BSON_SKIP_BYTES 9
/**
*	@class store
*   @brief A class containing the various data, logic, journals required to reliably transmit or represent firmware driven variables graphically.
* 		   Let's start with the summary, because if you're reading this it's probably not of your own free will,
* 		   this class is designed as a <b>Centralized Application State for data<b/>,  similar to the ideology that is widely used in distributed systems (especially speaking of JS, Vue, React, etc...)
*  		   @b HOWEVER it doesnt have some arguably annoying features of some of those systems, such as "Fold"/"Reduce" States or immutable data. This was choosen both due to personal experience 
* 		   and due to the fact that this is a real time system, and the overhead of having to copy the data to a new object every time it changes is not worth it. 
* 		   This class is intended to also be flexible and be able to tolerate future changes of communication protocol such as SPI, etc... 
*		   However since its not on the horizon for now theres no overhead or implementation for other protocols of communication.
**/
class store: public QObject{
    Q_OBJECT;
	// TODO well now its too late to remove this, but check what this does. Its a leftover of learning, and was presumed to be boylerplate
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
        explicit store(QString dev="", QSerialPort::BaudRate baud = QSerialPort::Baud115200, QObject *parent = nullptr);
		~store();
		
		//getters and setters
		QSerialPort::BaudRate getBaudRate() const;
		//getters wire variables
		int getRpm() const;
		int getEngineTemperature() const;
		float getBatteryVoltage() const;
		int getVehicleSpeed() const;
		//setters wire variables
		void setRpm(int rpm);
		void setEngineTemperature(int engineTemperature);
		void setBatteryVoltage(float batteryVoltage);
		void setVehicleSpeed(int vehicleVelocity);
		#ifdef __LART_T14__
			int getGearShift() const;			
			float getOilPressure() const;
			float getOilTemperature() const;
			int getDataLoggerStatus() const;
			float getLambda() const;
			int getTcSlip() const;
			int getTcLaunch() const;
			void setGearShift(int gearShift);
			void setOilPressure(float oilPressure);
			void setOilTemperature(float oilTemperature);
			void setDataLoggerStatus(int dataLoggerStatus);
			void setLambda(float lambda);
			void setTcSlip(int tcSlip);
			void setTcLaunch(int tcLaunch);

		#endif
		#ifdef __LART_T24__
			float getSoc() const;
			float getBatteryTemperature() const;
			int getInverterTemperature() const;
			short getPower() const;
			int getLapTime() const;
			short getLapCount() const;
                        short getHV() const;

                        //int getTyreTemperature() const;

                        void setSoc(float soc);
			void setBatteryTemperature(float batteryTemperature);
			void setInverterTemperature(int inverterTemperature);
			void setPower(short power);
			void setLapTime(int lapTime);
			void setLapCount(short lapCount);
                        void setHV(short hv);
			//void setTyreTemperature(int tyreTemperature);
		#endif

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
		void engineTemperatureChanged(int newEngineTemperature, int oldEngineTemperature);
		void batteryVoltageChanged(float newBatteryVoltage, float oldBatteryVoltage); 
		void vehicleSpeedChanged(int newVehicleSpeed, int oldVehicleSpeed);
		#ifdef __LART_T14__
			void gearShiftChanged(int newGearShift, int oldGearShift);
			void oilTemperatureChanged(float newOilTemperature, float oldOilTemperature); 
			void oilPressureChanged(float newOilPressure, float oldOilPressure); 
			void dataLoggerChanged(int newDataLogger, int oldDataLogger); 
			void lambdaChanged(float newLambda, float oldLambda); 
			void tcSlipChanged(int newTCSlip, int oldTCSlip); 
			void tcLaunchChanged(int newTcLaunch, int oldTcLaunch);  
		#endif
		#ifdef __LART_T24__
			void socChanged(float newSoc, float oldSoc);
			void batteryTemperatureChanged(float newBatteryTemperature, float oldBatteryTemperature);
			void inverterTemperatureChanged(int newInverterTemperature, int oldInverterTemperature);
			void powerChanged(short newPower, short oldPower);
			void lapTimeChanged(QTime newLapTime, QTime oldLapTime);
			void diffLapTimeChanged(QTime newDiffLapTime, QTime oldDiffLapTime);
			void lapCountChanged(short newLapCount, short oldLapCount);
                        void hvChanged(short newHV, short oldHV );

                        //void tyreTemperatureChanged(int newTyreTemperature, int oldTyreTemperature);
		#endif

    private:
		QSerialPort::BaudRate baud;
        QFile errorLog;
		int m_rotationsPerMinute=0;
		int m_engineTemperature=0;
		float m_batteryVoltage=0;
		int m_vehicleVelocity=0;
		#ifdef __LART_T14__
			int m_gearShift=0;
			float m_oilPressure=0;
			float m_oilTemperature=0;
			int m_dataLoggerStatus=0;
			float m_lambdaMixtureAirFuel=0;
			int m_tractionSlip=0;
			int m_tractionLaunch=0;
		#endif
		#ifdef __LART_T24__
			float m_stateOfCharge=0;
			float m_batteryTemperature=0;
			int m_inverterTemperature=0;
			short m_power=0;
			int m_lapTime=0;
			short m_lapCount=0;
			short m_highVoltage=0; 
			//int m_tyreTemperature=0;
		#endif

        
        
        
        
        

};

// Logging Macros
#define __LART_STORE_SETRPM_ERROR__ "store::setRpm(int rpm)->Rpm is negative"
#define __LART_STORE_SETGEARSHIFT_ERROR__ "store::setGearShift(int gearShift)->GearShift is out of bounds"

#endif // STORE_H
