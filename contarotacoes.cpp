#include "contarotacoes.h"

ContaRotacoes::ContaRotacoes( QWidget *parent)
    : QWidget(parent){
    m_value = 12000;
	m_maxValue=MAX_ROTATIONS_DEFAULT;
}
void ContaRotacoes::paintEvent(QPaintEvent *event){
   //padding of the line
   const int padding = 10; 
   const int size = width()-(padding*2);

   QPainter painter(this);
   painter.setRenderHint(QPainter::Antialiasing);
   //draw an arc from -45 degrees to 225 degrees
   painter.setPen(QPen(Qt::black, 10));
   painter.setBrush(Qt::NoBrush); 
   drawContaRotacoes(painter, size, padding);
   drawRotacoesText(painter, size, padding);

  


   


}


int ContaRotacoes::maxValue() const{
	return m_maxValue;
}
void ContaRotacoes::setMaxValue(int maxValue){
	if(m_maxValue < 0){
		m_maxValue = 0;
	}
	m_maxValue = maxValue;
	emit maxValueChanged(maxValue);
}





void ContaRotacoes::drawContaRotacoes(QPainter &painter, int size, int padding=10){

	//normal matrix
   painter.resetTransform();
	//minimum whiper of the arc
   const int minPhi = -45*16;
   //maximum whiper of the arc
   const int maxPhi = 270*16;
   double ratio = ((double)m_value)/((double)m_maxValue);
   
   int currentPhi =((int)((double)(ratio)*(maxPhi-minPhi)))+minPhi;

   painter.translate(size,0);
   painter.scale(-1.0, 1.0);
   //due to the mirrowing of the arc padding is weird
   painter.drawArc(-padding,height()/3, size, size, minPhi, maxPhi);
   painter.setPen(QPen(Qt::red, 4));
   painter.drawArc(-padding, height()/3, size, size,minPhi, currentPhi);
}
void ContaRotacoes::drawRotacoesText(QPainter &painter, int size, int padding=10){
	//normal matrix
   painter.resetTransform();

   //draw the text
   painter.setPen(QPen(Qt::black, 3));
   
   //make the font size scale with the width of the widget
   auto fontSize= width()/padding;

   QFont font = painter.font();

   font.setPointSizeF(fontSize);
   painter.setFont(font);
   painter.drawText(QRect(padding,height()/6,size,fontSize), Qt::AlignCenter, QString::number(m_value));
}
