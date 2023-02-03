#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./store.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	store* q = new store('a'); 
    q->fetchmessage();
	//grab the QObject which name is "label_mudancas"
	QObject *label_mudancas = this->findChild<QObject*>("label_mudancas");
	//cast it to a QLabel
	QLabel *label_mudancas_cast = qobject_cast<QLabel*>(label_mudancas);
	//cast character a to a const *QString 
	QString b = QString::fromStdString(std::string(1,q->a));

    label_mudancas_cast->setText(b);
}

MainWindow::~MainWindow()
{
    delete ui;
}

