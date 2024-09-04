#ifndef CONTAMUDANCAS_H
#define CONTAMUDANCAS_H

#include "contarotacoes.h"
#include <QObject>
#include <QWidget>
#include <QString>
#include <QColor>

class ContaMudancas : public ContaRotacoes{
    Q_OBJECT
	public:
		explicit ContaMudancas( QWidget *parent = nullptr);
		~ContaMudancas();
		void paintEvent(QPaintEvent *event) override;
		int getVisibleMudanca();
		QColor getGraphicColorWhipper() override;
		int getPositionFromVariationSlope() override;
		QString getGraphicalTextMudanca(int a);

	protected:

		int m_mudanca=0;
	public slots:
		void handleChangedMudanca(int newValue, int oldValue);
};

#endif // CONTAMUDANCAS_H
