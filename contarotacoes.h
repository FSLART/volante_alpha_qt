#ifndef CONTAROTACOES_H
#define CONTAROTACOES_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QPainter>
#define MAX_ROTATIONS_DEFAULT 20000
class ContaRotacoes : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int  maxValue READ maxValue WRITE setMaxValue NOTIFY maxValueChanged)
public:
    explicit ContaRotacoes( QWidget *parent = nullptr);
	int maxValue() const;
    void setMaxValue(int maxValue);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
	void drawContaRotacoes(QPainter &painter, int size, int padding);
	void drawRotacoesText(QPainter &painter, int size, int padding);
	int m_maxValue=MAX_ROTATIONS_DEFAULT;
	int m_value;
signals:
   void maxValueChanged(int maxValue);

};

#endif // CONTAROTACOES_H
