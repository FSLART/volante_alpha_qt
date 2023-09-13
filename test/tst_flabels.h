#include "aux.h"
#ifndef TST_FLABELS_H
#include <QFile>
#include <QObject>
#include <QTest>
#include <QDebug>
#include <QString>
#include "../src/mainwindow.h"
#include "aux.h"
// TODO: this will likely fail in the future
#include "../src/references/system_var.h"

class Tst_flabels : public QObject {
	Q_OBJECT;
	public: 
		AuxSingleton instance;

	protected:
		MainWindow ui; 
		store * pstore;
		
	private slots:
		void setup();
		void checkFLabelsLogicalChange();
		void cleanup();
};
#endif // TST_FLABELS_H
