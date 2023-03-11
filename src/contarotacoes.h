#ifndef CONTAROTACOES_H
#define CONTAROTACOES_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QPainter>
#include "mainwindow.h"

#define MAX_ROTATIONS_DEFAULT 20000
class ContaRotacoes : public QWidget
{
    Q_OBJECT	
	public:
		explicit ContaRotacoes( QWidget *parent = nullptr);
		int getValue () const;
		int getMaxValue () const;
	protected:
		void paintEvent(QPaintEvent *event) override;

	private:
		void drawContaRotacoes(QPainter &painter, int size, int padding);
		void drawRotacoesText(QPainter &painter, int size, int padding);
		int m_maxValue=MAX_ROTATIONS_DEFAULT;
		int m_value;

	public slots:
		void handleChangedValue (int newValue, int oldValue);
	
};

#endif // CONTAROTACOES_H
