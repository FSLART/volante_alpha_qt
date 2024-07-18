                        /**
+ @file flabel.cpp
* @brief This file contains all the logic that customizes the behaviour of most Text Labels on screen.
* @see flabel.h
* @author João Vieira
* This piece of software was developed for the T24e project belonging to the LART Team
*/
#include "flabel.h"
/**
* @brief Constructor for the FLabel Class
*/
FLabel::FLabel(QWidget *parent) : QLabel(parent){
	
}
/**
* @brief Handles the visual representation of a QString value
* @param newValue The new value, that will be displayed
* @param oldValue The old value
**/
void FLabel::setVisual(QString newValue, QString oldValue=""){
	//TODO Out of time, Out of touch.
	(void)oldValue;
	this->setText(newValue);
}
/**
 * @brief TODO
 **/
void FLabel::setAveragedVisualChangeSec(float newValue, float oldValue){
    this->_counts++;
    _alpha = (float)((this->_counts * newValue) + (1.0 - this->_counts) * oldValue);
    float truenewvalue = (float)((float)(_alpha/_counts));
    auto rateOfChange = (truenewvalue-oldValue)/oldValue * 100;
    this->setText(QString::number(truenewvalue)+" / "+QString::number(newValue)+ " W"+" ("+QString::number(rateOfChange)+"%)");
}

void FLabel::setAveragedVisualChangeSec(short newValue, short oldValue){
    this->_counts++;
    this->_alpha = (float)( (this->_counts * newValue) + (1.0 - this->_counts) * _alpha);
    short truenewvalue = ((short)(_alpha/_counts));
    short rateOfChange =100;
    if(oldValue!=0){
        rateOfChange = ((short)((truenewvalue-oldValue)/oldValue)) * 100;
    }

        //TODO URGENT fix units
    this->setText(QString::number(truenewvalue)+" / "+QString::number(oldValue)+ " W"+" ("+QString::number(rateOfChange)+"%)");
}
/**
* @brief Handles the visual representation of an int value
* @param newValue The new value, that will be displayed
* @param oldValue The old value, intended for animation purposes, defaults to "0"
**/
void FLabel::setVisual(int newValue, int oldValue=0){
	//TODO Out of time, Out of touch.
	(void)oldValue;
	this->setText(QString::number(newValue));
}

/**
* @brief Handles the visual representation of a short value, Calls setVisual(int, int)
* @param newValue The new value, that will be displayed
* @param oldValue The old value, intended for animation purposes, defaults to "0"
**/
void FLabel::setVisual(short newValue, short oldValue=0){
	//TODO Out of time, Out of touch.
	(void)oldValue;
	this->setVisual((int)newValue, (int)oldValue);
}
/**
* @brief Handles the visual representation of a float value
* @param newValue The new value, that will be displayed
* @param oldValue The old value, intended for animation purposes, defaults to "0"
**/
void FLabel::setVisual(float newValue, float oldValue=0){
	//TODO Out of time, Out of touch.
	(void)oldValue;
	this->setText(QString::number(newValue));
}
/**
* @brief Handles the visual representation of a QTime value
* @param newValue The new value, that will be displayed 
* @param oldValue The old value, intended for animation purposes, defaults to "0"
**/
void FLabel::setVisual(QTime newValue, QTime oldValue=QTime(0,0,0,0)){
	//TODO Out of time, Out of touch.
	(void)oldValue;
	 
	int msfromStart= newValue.msecsSinceStartOfDay(); 
	
	if(msfromStart<0){
		//turn newValue positive and prepend it 
		newValue=QTime::fromMSecsSinceStartOfDay(abs(msfromStart));
		this->setText("-"+newValue.toString("mm:ss.zzz"));
		return;

	}
	
	this->setText(newValue.toString("mm:ss.zzz"));
}
