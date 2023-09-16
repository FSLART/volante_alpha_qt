#ifndef TST_CONTAMUDANCAS_H
#define TST_CONTAMUDANCAS_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include <QProcess>
#include "tst_contarotacoes.h"
#include "../src/contamudancas.h"
#include "../src/store.h"
#include "../src/mainwindow.h"
class Tst_contamudancas : public Tst_contarotacoes
{
    Q_OBJECT
	public: 
		Tst_contamudancas() = default;
		~Tst_contamudancas() = default;
    private slots:
        //void checkRpmChangesFromStoreToGraphicText();
		void checkMudancaChangesFromStore();
		void checkGearShiftEncoding();
		void checkRPMErrorLogging();

};

#endif // TST_CONTAMUDANCAS_H
