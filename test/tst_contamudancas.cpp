#include "tst_contamudancas.h"


void Tst_contamudancas::checkRpmChangesFromStoreToGraphicText(){
	//TODO find a way to solve bellow apparently it crashes violently with a permission dennied in this context
	QProcess socat; 
	socat.startDetached("socat pty,raw,echo=0,b115200,link=/tmp/banana,  pty,raw,echo=0,b115200,link=/tmp/tango");
    MainWindow ui= MainWindow(nullptr,"/tmp/banana");

	//ui.add a widget contarotacoes with name _test
    ContaMudancas _test = ContaMudancas(&ui);
    _test.setObjectName("_test");
	
    auto s = ui.getStore();
	s->setRpm(100);
    int a = ui.findChild<ContaMudancas*>("_test")->getValue();


    QCOMPARE(a, 100);
}


