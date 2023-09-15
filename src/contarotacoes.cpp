#include "contarotacoes.h"
static store * store_pnt = nullptr;
ContaRotacoes::ContaRotacoes( QWidget *parent)
	 : QWidget(parent){
	 m_value=0;

	 //q is a static variable that you get from calling the static function getStore() from mainwindow
	//find MainWindow and get the store
	try{
		MainWindow* w = qobject_cast<MainWindow*>(parent->parent());
		store_pnt=w->getStore();
		connect(store_pnt,&store::rpmChanged, this, &ContaRotacoes::handleChangedValue);  
	}catch(...){
		//TODO proper notices
		qDebug() << "Error: ContaRotacoes::ContaRotacoes( QWidget *parent) failed to connect to store";

	}
	m_maxValue=MAX_ROTATIONS_DEFAULT;

	
}
ContaRotacoes::~ContaRotacoes(){
	disconnect(store_pnt,&store::rpmChanged, this, &ContaRotacoes::handleChangedValue);

}
void ContaRotacoes::paintEvent(QPaintEvent *event){
	//remove the warning for unused *event
	(void)event;
	
	//padding of the line
	const int padding = 10; 
	const int size = width()-(padding*2);
        ;
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	const auto group = QPalette::Active;
  	const auto role = QPalette::Text;


    auto palette=this->palette();

    /*Get Brush */
    painter.setPen(QPen(palette.color(group, role),25));
	

	painter.setBrush(Qt::NoBrush); 
	drawContaRotacoes(painter, size, padding);
	drawRotacoesText(painter, size, padding);
}

void ContaRotacoes::handleChangedValue(int newValue, int oldValue){
	//remove unused warning
	(void)oldValue;
	// value< m_maxValue || this should be in the if for better data quality but in case of a ill fate, it might be interesting to see the values
	if( newValue >= 0){
		this->m_value = newValue;
		this->update();
	}
	
}

void ContaRotacoes::drawContaRotacoes(QPainter &painter, int size, int padding=10){

	//normal matrix
	painter.resetTransform();
	//minimum whipper of the arc
	//const int minPhi = -45*16;
	const int minPhi = -720;
	//maximum whipper of the arc
	//const int maxPhi = 270*16;
	const int maxPhi = 4320; 
	

	painter.translate(size,0);
	painter.scale(-1.0, 1.0);
	//due to the mirrowing of the arc padding is weird
	painter.drawArc(-padding, height()/3, size, size, minPhi, maxPhi);
	QColor pen = getGraphicColorWhipper();
	painter.setPen(QPen(pen, 20));
	painter.drawArc(-padding, height()/3, size, size,minPhi, getPositionFromVariationSlope());
}
void ContaRotacoes::drawRotacoesText(QPainter &painter, int size, int padding=10){
	//normal matrix
	painter.resetTransform();
	const auto group = QPalette::Active;
  	const auto role = QPalette::Text;


    auto palette=this->palette();

	//draw the text
    painter.setPen(QPen(palette.color(group, role), 3));
	
	//make the font size scale with the width of the widget
	auto fontSize= width()/padding;

	QFont font = painter.font();

	font.setPointSizeF(fontSize);
	painter.setFont(font);
	painter.drawText(QRect(padding,height()/6,size,fontSize), Qt::AlignCenter, QString::number(m_value));
}

int ContaRotacoes::getValue () const{
	return m_value;
}
int ContaRotacoes::getMaxValue () const{
	return m_maxValue;
}
QColor ContaRotacoes::getGraphicColorWhipper(){
	//TODO EXAMPLE CODE THIS IS SUPPOSED TO BE UNDER SUPERVISION
	int red=std::floor(255* (1-(m_value/m_maxValue)));
	int blue=std::floor(255*m_value/m_maxValue);
	return QColor(red,0,blue);
}
int ContaRotacoes::getPositionFromVariationSlope(){
	//minimum whipper of the arc
	//const int minPhi = -45*16;
	const int minPhi = -720;
	//maximum whipper of the arc
	//const int maxPhi = 270*16;
	const int maxPhi = 4320; 
	//TODO EXAMPLE CODE THIS IS SUPPOSED TO BE UNDER SUPERVISION
	double ratio = ((double)m_value)/((double)m_maxValue);
	int currentPhi =((int)((double)(ratio)*(maxPhi-minPhi)));
	return currentPhi;
}
