#ifndef CONTAMUDANCAS_H
#define CONTAMUDANCAS_H

#include "contarotacoes.h"
#include <QObject>
#include <QWidget>
#include <QString>

class ContaMudancas : public ContaRotacoes{
    Q_OBJECT
	public:
		explicit ContaMudancas( QWidget *parent = nullptr);
		void paintEvent(QPaintEvent *event) override;
		int getVisibleMudanca();
		QString getGraphicalTextMudanca(int a);
	protected:

		int m_mudanca=0;
	public slots:
		void handleChangedMudanca(int newValue, int oldValue);
};

#endif // CONTAMUDANCAS_H
