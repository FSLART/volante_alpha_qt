/**
* @file contamudancas.cpp
* @brief This file contains additional logic to a child of the RPM meter(contarotacoes),
*		 that logic being mostly used for displaying the current gear shift.
* 		 In its current form as of 16/09/2023, its also provides a non linear graphical representation of the RPM meter.
* @see contarotacoes.h
* @author João Vieira
* This piece of software was developed for the T24e project belonging to the LART Team
**/
#include "contamudancas.h"
#include "contarotacoes.h"
#include "mainwindow.h"
#include <cmath>

static store * store_pnt = nullptr;
/**
* @brief Constructor for the ContaMudancas Class, Responsible for the @b connection off both Gear and RPM variables. 
**/
ContaMudancas::ContaMudancas( QWidget *parent)
    : ContaRotacoes(parent){
    this->m_mudanca=0;
    try{
        MainWindow* w = qobject_cast<MainWindow*>(parent->parent());
        store_pnt=w->getStore();
		
		//Bellow is a macro as even if its not used in the graphical window, it would require a declaration to be made that isn't present on T24e. 
		#ifdef __LART_T14__
        	connect(store_pnt,&store::gearShiftChanged, this, &ContaMudancas::handleChangedMudanca);
		#endif
    }catch(...){
		//TODO proper notices
		qDebug() << "Error: ContaMudancas::ContaMudancas( QWidget *parent) failed to connect to store";

	}

}

// Value used for padding, Unless you are messing with the graphical design DO NOT CHANGE THE VALUE. 
const int padding = 10; 

/**
* @brief Handles the visual representation of the widget. Calls its parent, ContaRotacoes::paintEvent and then draws the gear shift.
* @param event The event that triggered the paintEvent
* @see ContaRotacoes::paintEvent
**/
void ContaMudancas::paintEvent(QPaintEvent *event){
    ContaRotacoes::paintEvent(event);
	
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	const auto group = QPalette::Active;
  	const auto role = QPalette::Text;


    auto palette=this->palette();
	painter.setPen(QPen(palette.color(group, role)));
    painter.setFont(QFont("Source Code Pro", width()/padding*4));
	//rect add padding vertical 
    painter.drawText(this->rect().adjusted(0, padding*4, 0, 0), Qt::AlignCenter, getGraphicalTextMudanca(m_mudanca));
	
}
/**
* @brief Returns the current gear shift
* @return The current gear shift
**/
int ContaMudancas::getVisibleMudanca(){
	return m_mudanca;
}
/**
* @brief Returns the text that should be displayed on the gear shift, as of 16/09/2023, its mostly a demo.
* @param a The gear shift
* @return The text that should be displayed on the gear shift
**/
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
/**
* @brief Slot callback for the gear shift change, if valid, updates both variables, and orders a graphical redraw.
**/
void ContaMudancas::handleChangedMudanca(int newValue, int oldValue){
	//remove unused warning
	(void)oldValue;

	// value< m_maxValue || this should be in the if for better data quality but in case of a ill fate, it might be interesting to see the values
	if( newValue >= -1){
		this->m_mudanca = newValue;
		this->update();
	}
}
/**
* @brief Destructor for the ContaMudancas Class, Responsible for the @b disconnection of signals and its slots. 
**/
ContaMudancas::~ContaMudancas(){
	#ifdef __LART_T14__
	disconnect(store_pnt,&store::gearShiftChanged, this, &ContaMudancas::handleChangedMudanca);
	#endif
}

/**
* @brief Returns the color that should be used for the RPM whipper, as of 16/09/2023, its mostly a demo. 
*		 @b NON-PURE function, it uses the current state of the object...
* @return The color that should be used for the RPM whipper
* @see ContaRotacoes::getGraphicColorWhipper()
**/
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
/**
* @brief Returns the position of the RPM whipper.
* @return The position in Degrees*16 of the RPM whipper
* @see ContaRotacoes::getPositionFromVariationSlope()
* @see QPainter::drawArc()
**/
int ContaMudancas::getPositionFromVariationSlope(){
	//call the parent function
	int currentPhi=ContaRotacoes::getPositionFromVariationSlope();
	return currentPhi;
}
