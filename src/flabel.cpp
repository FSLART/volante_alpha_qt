#include "flabel.h"

FLabel::FLabel(QWidget *parent) : QLabel(parent){
	
}

void FLabel::setVisual(int newValue, int oldValue){
	this->setText(QString::number(newValue));
}
void FLabel::setVisual(short newValue, short oldValue){
	this->setVisual((int)newValue, (int)oldValue);
}
void FLabel::setVisual(float newValue, float oldValue){
	this->setText(QString::number(newValue));
}
/*Overload for time*/
void FLabel::setVisual(QTime newValue, QTime oldValue){
	this->setText(newValue.toString("mm:ss.zzz"));
}
void FLabel::setVisual(QString newValue, QString oldValue){
	this->setText(newValue);
}
