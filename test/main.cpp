#include <QApplication>
#include <QTest>
#include <QList>
#include <QObject>
#include <cstdint>
//write me a macro that includes all .h files in the test folder
#include "tst_serialport.h"
#include "tst_contarotacoes.h"

typedef struct{
	uint32_t id;
    QString name;
	QObject* test;

}suite;

suite fullsuite [] = {
    {0,QString("SerialPort"),new Tst_serialport},
    {1,QString("ContaRotacoes"),new Tst_contarotacoes},
};
int main(int argc, char *argv[]){
    QApplication app(argc, argv);
	int status = 0;
	for(int i = 0; i < sizeof(fullsuite)/sizeof(suite); i++){
		status |= QTest::qExec(fullsuite[i].test, argc, argv);
	}
	return status;
	
}
