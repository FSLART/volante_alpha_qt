#include "flabel.h"

FLabel::FLabel(QWidget *parent) : QLabel(parent){
	
}

void FLabel::setVisual(int newValue, int oldValue){
	this->setText(QString::number(newValue));
}
void FLabel::setVisual(float newValue, float oldValue){
	this->setText(QString::number(newValue));
}
void FLabel::setVisual(QString newValue, QString oldValue){
	this->setText(newValue);
}
