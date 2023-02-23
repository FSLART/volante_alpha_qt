#ifndef TST_SERIALPORT_H
#define TST_SERIALPORT_H
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
class Tst_serialport : public QObject
{
    Q_OBJECT
    private:
		QProcess socat;
        store * _store;
		void tangoWriteSetup();
		int randomInt(int offset, int n);
		
    private slots:
        void sanityCheck();
		void checkSendMessage();
		void storeMessage();
        void bsonTest();
		void partitionedSlowBsonMessage();
		void prependingTrash();
		void suffixingTrash();
		void updatingRPM();
        void closeHandle();

};
#endif // TST_SERIALPORT_H
