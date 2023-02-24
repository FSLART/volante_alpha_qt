#include "tst_contarotacoes.h"


void Tst_contarotacoes::checkRpmChangesFromStoreToGraphicText(){
	//TODO find a way to solve bellow apparently it crashes violently with a permission dennied in this context
	QProcess socat; 
	socat.startDetached("socat pty,raw,echo=0,b115200,link=/tmp/banana,  pty,raw,echo=0,b115200,link=/tmp/tango");
    MainWindow ui= MainWindow(nullptr,"/tmp/banana");

	//ui.add a widget contarotacoes with name _test
    ContaRotacoes _test = ContaRotacoes(&ui);
    _test.setObjectName("_test");
	

    auto s = ui.getStore();
	s->setRpm(100);
    int a = ui.findChild<ContaRotacoes*>("_test")->getValue();


    QCOMPARE(a, 100);
}


