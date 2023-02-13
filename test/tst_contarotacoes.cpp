#include "tst_contarotacoes.h"


void Tst_contarotacoes::checkRpmChangesFromStoreToGraphicText(){
	MainWindow ui;
    auto s = ui.getStore();
	s->setRpm(100);
    int a = ui.findChild<ContaRotacoes*>("contarotacoes")->getValue();
    QCOMPARE(a, 100);
}


QTEST_MAIN(Tst_contarotacoes)
