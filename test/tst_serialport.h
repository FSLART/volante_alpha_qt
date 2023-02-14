#ifndef TST_SERIALPORT_H
#define TST_SERIALPORT_H
#include <QObject>
#include <QTest>
#include <QDebug>
#include <QString>
#include <sched.h>
#include "../src/store.h"
#include "../src/mainwindow.h"
class Tst_serialport : public QObject
{
    Q_OBJECT
    private:
        FILE * handle;
		QString banana;
		QString tango;
		void launchSocat();
		__pid_t pid;
	private slots:
        void sanityCheck();
        void closeHandle();
};
#endif // TST_SERIALPORT_H
