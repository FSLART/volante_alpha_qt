#include "tst_contarotacoes.h"
#include "qcoreapplication.h"



void Tst_contarotacoes::setupTest(){
	program="socat";
	args = QStringList();

	args.append("pty,raw,echo=0,b38400,link=/tmp/banana,");
	args.append("pty,raw,echo=0,b38400,link=/tmp/tango");
}
void Tst_contarotacoes::checkRpmChangesFromStoreToGraphicText(){
	//TODO find a way to solve bellow apparently it crashes violently with a permission dennied in this context
    QProcess socat;

        socat.startDetached(program, args);
    MainWindow ui= MainWindow(nullptr,"/tmp/banana");

	//ui.add a widget contarotacoes with name _test
    ContaRotacoes _test = ContaRotacoes(&ui);
    _test.setObjectName("_test");
	

    auto s = ui.getStore();
	s->setRpm(100);
    int a = ui.findChild<ContaRotacoes*>("_test")->getValue();
    socat.kill();

    QCOMPARE(a, 100);
}


void Tst_contarotacoes::checkRotationErrorLogging(){
	QProcess socat;
    socat.startDetached(program, args);
    MainWindow ui= MainWindow(nullptr,"/tmp/banana");

	
	
    //open the most recent file in the folder
	QDir dir = QDir::currentPath();
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Time);
    QFileInfoList list = dir.entryInfoList();
    QFile file(list.first().absoluteFilePath());
    file.open(QIODevice::ReadOnly);
    file.seek(file.size());

	QTextStream in(&file);
	ui.getStore()->setRpm(-1);
	QString line = in.readLine();
	socat.kill();
	file.close(); 
	QVERIFY(line.contains(__LART_STORE_SETRPM_ERROR__));
	
	

}
