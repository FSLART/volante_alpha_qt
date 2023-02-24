#include "contamudancas.h"
#include "contarotacoes.h"
#include "mainwindow.h"

ContaMudancas::ContaMudancas( QWidget *parent)
    : ContaRotacoes(parent){
    this->m_mudanca=0;
    try{
        MainWindow* w = qobject_cast<MainWindow*>(parent->parent());
        connect(w->getStore(),&store::gearShiftChanged, this, &ContaMudancas::handleChangedMudanca);
    }catch(...){
		//TODO proper notices
		qDebug() << "Error: ContaMudancas::ContaMudancas( QWidget *parent) failed to connect to store";

	}

}


const int padding = 10; 

void ContaMudancas::paintEvent(QPaintEvent *event){
    ContaRotacoes::paintEvent(event);
	//draw a big QString centered in the middle of the widget with getGraphicalTextMudanca(m_mudanca)
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", width()/padding*4));
	//rect add padding vertical 
    painter.drawText(this->rect().adjusted(0, padding*4, 0, 0), Qt::AlignCenter, getGraphicalTextMudanca(m_mudanca));
	
}
int ContaMudancas::getVisibleMudanca(){
	return m_mudanca;
}
QString ContaMudancas::getGraphicalTextMudanca(int a){
	switch (a) {
		case 0:
			return QString("N");

		default:
		//a to QString
			return QString::number(a);
		
	
	}
}
void ContaMudancas::handleChangedMudanca(int newValue, int oldValue){
	// value< m_maxValue || this should be in the if for better data quality but in case of a ill fate, it might be interesting to see the values
	if( newValue >= -1){
		this->m_mudanca = newValue;
		this->update();
	}
}
