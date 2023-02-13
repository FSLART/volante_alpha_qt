#ifndef TST_CONTAROTACOES_H
#define TST_CONTAROTACOES_H

#include <QObject>

class tst_contarotacoes : public QObject
{
    Q_OBJECT
public:
    explicit tst_contarotacoes(QObject *parent = nullptr);

private slots:
	void sanityTest(); 

};

#endif // TST_CONTAROTACOES_H
