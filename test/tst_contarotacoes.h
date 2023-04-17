#ifndef TST_CONTAROTACOES_H
#define TST_CONTAROTACOES_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include <QProcess>
#include "../src/contarotacoes.h"
#include "../src/store.h"
#include "../src/mainwindow.h"
class Tst_contarotacoes : public QObject
{
    Q_OBJECT
    public:
        QString program;
        QStringList args;
    private slots:
        void setupTest();
        void checkRpmChangesFromStoreToGraphicText();
        void checkRotationErrorLogging();
};

#endif // TST_CONTAROTACOES_H
