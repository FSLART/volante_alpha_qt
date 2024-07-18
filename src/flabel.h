/**
* @file flabel.h
* @brief This file contains the declaration of the FLabel class
* @see flabel.cpp
* @author João Vieira
* This piece of software was developed for the T24e project belonging to the LART Team
*/
#ifndef FLABEL_H
#define FLABEL_H
#include <QLabel>
#include <QObject>
#include <QString>
#include <QDebug>
#include "store.h"
/**
* @brief The FLabel class is meant to derive from QLabel but provide some customizations and extra functionality to it
**/
class FLabel : public QLabel{
	Q_OBJECT;
	public:
            explicit FLabel(QWidget *parent = nullptr);

	public slots:
		void setVisual(int newValue, int oldValue);
		void setVisual(QTime newValue, QTime oldValue); 
		void setVisual(float newValue, float oldValue);
                void setVisual(short newValue, short oldValue);
                void setVisual(QString newValue, QString oldValue);
                void setAveragedVisualChangeSec(float newValue,float oldValue);
                void setAveragedVisualChangeSec(short newValue,short oldValue);
        private:
                //used for rolling averages
                int _counts = 0;
                float _alpha = 0;


};
#endif // FLABEL_H
