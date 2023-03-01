#include "tst_contamudancas.h"


/*void Tst_contamudancas::checkRpmChangesFromStoreToGraphicText(){
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
    socat.terminate();
	
    QCOMPARE(a, 100);
}*/
void Tst_contamudancas::checkMudancaChangesFromStore(){
	//TODO find a way to solve bellow apparently it crashes violently with a permission dennied in this context
	QProcess socat; 
	socat.startDetached("socat pty,raw,echo=0,b115200,link=/tmp/banana,  pty,raw,echo=0,b115200,link=/tmp/tango");
	MainWindow ui= MainWindow(nullptr,"/tmp/banana");

	//ui.add a widget contarotacoes with name _test
	ContaMudancas _test = ContaMudancas(&ui);
	_test.setObjectName("_test");
	
	auto s = ui.getStore();
	s->setGearShift(1);
    int a = ui.findChild<ContaMudancas*>("_test")->getVisibleMudanca();
    socat.terminate();
	QCOMPARE(a, 1);
}

void Tst_contamudancas::checkGearShiftEncoding(){
    //TODO find a way to solve bellow apparently it crashes violently with a permission dennied in this context
    QProcess socat;
    socat.startDetached("socat pty,raw,echo=0,b115200,link=/tmp/banana,  pty,raw,echo=0,b115200,link=/tmp/tango");
    MainWindow ui= MainWindow(nullptr,"/tmp/banana");

    ContaMudancas _test = ContaMudancas(&ui);
	
	QCOMPARE(_test.getGraphicalTextMudanca(0), QString("N"));
	for(int i = 1; i < 6; i++){
        QCOMPARE(_test.getGraphicalTextMudanca(i),QString::number(i));
	}
    socat.terminate();
}

void Tst_contamudancas::checkErrorLogging(){
	QProcess socat;
    socat.startDetached("socat pty,raw,echo=0,b115200,link=/tmp/banana,  pty,raw,echo=0,b115200,link=/tmp/tango");
    MainWindow ui= MainWindow(nullptr,"/tmp/banana");

	ui.getStore()->setGearShift(-1);
	ui.getStore()->setGearShift(100);
    //open the most recent file in the folder
	socat.terminate();

	QDir dir = QDir::currentPath();
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Time);
    QFileInfoList list = dir.entryInfoList();
    QFile file(list.first().absoluteFilePath());
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
	QString line = in.readLine();
	
	QString line2 = in.readLine();
	QVERIFY(line.contains(__FSIPLEIRIA_STORE_SETGEARSHIFT_ERROR__));
	QVERIFY(line2.contains(__FSIPLEIRIA_STORE_SETGEARSHIFT_ERROR__));
	

}

