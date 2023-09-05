#include "contamudancas.h"
#include "contarotacoes.h"
#include "mainwindow.h"
#include <cmath>
static store * store_pnt = nullptr;
ContaMudancas::ContaMudancas( QWidget *parent)
    : ContaRotacoes(parent){
    this->m_mudanca=0;
    try{
        MainWindow* w = qobject_cast<MainWindow*>(parent->parent());
        store_pnt=w->getStore();
        connect(store_pnt,&store::gearShiftChanged, this, &ContaMudancas::handleChangedMudanca);
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
	const auto group = QPalette::Active;
  	const auto role = QPalette::Text;


    auto palette=this->palette();
	painter.setPen(QPen(palette.color(group, role)));
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
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			return QString::number(a);
		default:
			return QString("?");
		
	
	}
}
void ContaMudancas::handleChangedMudanca(int newValue, int oldValue){
	//remove unused warning
	(void)oldValue;

	// value< m_maxValue || this should be in the if for better data quality but in case of a ill fate, it might be interesting to see the values
	if( newValue >= -1){
		this->m_mudanca = newValue;
		this->update();
	}
}
ContaMudancas::~ContaMudancas(){
	disconnect(store_pnt,&store::gearShiftChanged, this, &ContaMudancas::handleChangedMudanca);
}
QColor ContaMudancas::getGraphicColorWhipper(){
    switch (m_mudanca) {
			case 0:
                {
                double ratio = (double)(m_value)/m_maxValue;
                    int red=std::floor(255* (1-ratio));
                    int blue=std::floor(255*ratio);
					return QColor(red,0,blue);
				}
                break;
			case 1: 
			case 2: 
				if(m_value<400){
					//return deep red
					return QColor(200,0,50);
				}else if(m_value<800){
					return QColor(255,0,0);
				}else if(m_value<1200){
					//return light green
					return QColor(0,230,0);
				}else if(m_value<1300){
					//return light green
					return QColor(0,230,230);
				}else if(m_value<1400){
					//return light green
					return QColor(0,230,0);
				}else if(m_value<2000){
					//return light green
					return QColor(255,0,0);
				}else{
					return QColor(200,0,50);
				}
				break;
			case 3:
				if(m_value<400){
					//return deep red
					return QColor(200,0,50);
				}else if(m_value<800){
					return QColor(255,0,0);
				}else if(m_value<1200){
					//return light green
					return QColor(0,230,0);
				}else if(m_value<1300){
					//return light green
					return QColor(0,230,230);
				}else if(m_value<1400){
					//return light green
					return QColor(0,230,0);
				}else if(m_value<2000){
					//return light green
					return QColor(255,0,0);
				}else{
					return QColor(200,0,50);
				}
				break;
			case 4:
			case 5:
			case 6:
				if(m_value<400){
					//return deep red
					return QColor(200,0,50);
				}else if(m_value<800){
					return QColor(255,0,0);
				}else if(m_value<1200){
					//return light green
					return QColor(0,230,0);
				}else if(m_value<1300){
					//return light green
					return QColor(0,230,230);
				}else if(m_value<1400){
					//return light green
					return QColor(0,230,0);
				}else if(m_value<2000){
					//return light green
					return QColor(255,0,0);
				}else{
					return QColor(200,0,50);
				}
				break;
			default:
				return QColor(0,0,0);
				break;
			
        }

}
int ContaMudancas::getPositionFromVariationSlope(){
	const int minPhi = -720;
	//maximum whipper of the arc
	//const int maxPhi = 270*16;
	const int maxPhi = 4320; 
	
	//TODO EXAMPLE CODE THIS IS SUPPOSED TO BE UNDER SUPERVISION
	double ratio = ((double)m_value)/((double)m_maxValue);
   
   	int currentPhi =((int)((double)(ratio)*(maxPhi-minPhi)));
	return currentPhi;
}
