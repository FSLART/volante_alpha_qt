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
    Q_OBJECT;
    Q_PROPERTY(int  m_rotationsPerMinute READ getRpm WRITE setRpm NOTIFY rpmChanged);
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
		

	protected:
		int setupSerial();
		int closeSerial();
	signals:
		void rpmChanged(int newRpm, int oldRpm);

    private:
        int m_rotationsPerMinute;
        int m_gearShift;
        int m_engineTemperature;
        float m_oilPressure;
        float m_oilTemperature;
        float m_batteryVoltage;
		int m_vehicleVelocity;
		int m_dataLoggerStatus;
		//todo ask stuff about this
		float m_lambdaMixtureAirFuel; 
		int m_tractionSlip;
		int m_tractionLaunch;
};


#endif // STORE_H
