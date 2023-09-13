#include "tst_flabels.h"

void Tst_flabels::setup(){
	//create a MainWindww
	this->ui= MainWindow(nullptr, __LART_NULL_DEVICE_DESCRIPTOR__);
	//get the ui store
	pstore = this->ui.store;
	
}
void Tst_flabels::checkFLabelsLogicalChange(){
	pstore->setRpm(10);
	QCOMPARE(pstore->getRpm(), 10);
}
void Tst_flabels::cleanup(){

} 