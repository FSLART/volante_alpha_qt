#include "tst_contarotacoes.h"

tst_contarotacoes::tst_contarotacoes(QObject *parent)
    : QObject{parent}{
	
}

void tst_contarotacoes::sanityTest(){
	QVERIFY(1==1);
}