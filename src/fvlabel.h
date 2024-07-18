/**
* @file fvlabel.h
* @brief This file contains the declaration of the FVLabel class
* @see fvlabel.cpp
* @author João Vieira
* This piece of software was developed for the T24e project belonging to the LART Team
*/
#ifndef FVLABEL_H
#define FVLABEL_H
#include <QLabel>
#include "flabel.h"
#include <QObject>
#include <QString>
#include <QDebug>
#include "store.h"
/**
* @brief The FVLabel class is meant to derive from FLabel it will append text to denote the SI variable type such as m for meters for example
**/
class FVLabel : public FLabel{
	Q_OBJECT;
	public:
            explicit FVLabel(QWidget *parent = nullptr);

	public slots:
        protected:
                QString va;
        private:
                //used for rolling averages
                int _counts = 0;
                float _alpha = 0;


};
#endif // FVLABEL_H
