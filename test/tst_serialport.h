#ifndef TST_SERIALPORT_H
#define TST_SERIALPORT_H
#include <QFile>
#include <QObject>
#include <QTest>
#include <QDebug>
#include <QString>
#include <sched.h>
#include <QProcess>
#include <QRandomGenerator>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <fstream>
#include <QSerialPort>
#include "../src/store.h"
#include "../src/mainwindow.h"
#include "aux.h"
class Tst_serialport : public QObject {
    Q_OBJECT
    public:
        QString program;
        QStringList args;
        AuxSingleton* instance;
		Tst_serialport() = default;
		~Tst_serialport() = default;
    private:
		QProcess socat;
                store * _store;
		void tangoWriteSetup();

    private slots:
		void setup(); 
        void sanityCheck();
		void checkSendMessage();
		void storeMessage();
        void bsonTest();
		//void debugFailing(); 
        void prependingTrash();
		void partitionedSlowBsonMessage();
		void suffixingTrash();
		void updatingRPM();
        void closeHandle();

};
#endif // TST_SERIALPORT_H
