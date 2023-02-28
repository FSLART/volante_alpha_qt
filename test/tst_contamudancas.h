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
    private slots:
        //void checkRpmChangesFromStoreToGraphicText();
		void checkMudancaChangesFromStore();
		void checkGearShiftEncoding();
		void checkErrorLogging();
};

#endif // TST_CONTAMUDANCAS_H
