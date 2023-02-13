#ifndef STORE_H
#define STORE_H
#include <iostream>
#include <qserialport.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdio>
#include <QDebug>
#include <QSerialPort>
#include <thread>
#define DEFAULT_DEVICE "/dev/ttyACM0"
/* !
        \class store
        \brief Basicamente uma store contendo as vários dados necessários a serem transmitidos ou representados graficamente

		Comecemos com o resumo pois se estás a ler isto provavelmente não e de completa livre vontade,
		esta classe representa um ponto centralizado dos dados semelhante a ideologia que e muito usada em sistemas distribuídos(especialmente falando em JS, Vue, React, etc...)
		
		\fields
*/
class store: public QObject{
    Q_OBJECT
    public:
		char* dev=nullptr;
		QSerialPort* port=nullptr;
		void handleReadyRead();
		void handleError(QSerialPort::SerialPortError serialPortError);
		QByteArray serialLog;
		QByteArray lastMessage; 
        explicit store(char * dev=nullptr, QObject *parent = nullptr);
		~store();
		
		//getters and setters
		int getRpm() const;
		int getGearShift() const;
		float getEngineTemperature() const;
		float getOilPressure() const;
		float getOilTemperature() const;
		float getBatteryVoltage() const;
		float getVehicleVelocity() const;
		int getDataLoggerStatus() const;
		float getLambda() const;
		int getTcSlip() const;
		int getTcLaunch() const;
		void setRpm(int rpm);
		void setGearShift(int gearShift);
		void setEngineTemperature(float engineTemperature);
		void setOilPressure(float oilPressure);
		void setOilTemperature(float oilTemperature);
		void setBatteryVoltage(float batteryVoltage);
		void setVehicleVelocity(float vehicleVelocity);
		void setDataLoggerStatus(int dataLoggerStatus);
		void setLambda(float lambda);
		void setTcSlip(int tcSlip);
		void setTcLaunch(int tcLaunch);
		

	protected:
		int setupSerial();
		int closeSerial();
    private:
        int m_rotationsPerMinute;
        int m_gearShift;
        float m_engineTemperature;
        float m_oilPressure;
        float m_oilTemperature;
        float m_batteryVoltage;
		float m_vehicleVelocity;
		int m_dataLoggerStatus;
		//todo ask stuff about this
		float m_lambda; 
		int m_tcSlip;
		int m_tcLaunch;
};


#endif // STORE_H
