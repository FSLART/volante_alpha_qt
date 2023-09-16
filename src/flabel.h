#ifndef FLABEL_H
#define FLABEL_H
#include <QLabel>
#include <QObject>
#include <QString>
#include <QDebug>
#include "store.h"

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

};
#endif // FLABEL_H
