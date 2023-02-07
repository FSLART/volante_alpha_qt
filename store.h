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
	protected:
		int setupSerial();
		int closeSerial();
};


#endif // STORE_H
