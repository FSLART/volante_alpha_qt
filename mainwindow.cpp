#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./store.h"
#include <cstddef>
#include <qmainwindow.h>
#include "responsive_manager.h"
static responsive_manager manager;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
		manager = responsive_manager();
    	ui->setupUi(this);
        q = new store(nullptr);

        //grab the QObject which name is "label_mudancas"
        //QObject *label_mudancas = this->findChild<QObject*>("label_mudancas");
        //cast it to a QLabel
       // QLabel *label_mudancas_cast = qobject_cast<QLabel*>(label_mudancas);
        //cast character a to a const *QString
        //QString b = QString::fromStdString(std::string(1,q->mudança));

        //label_mudancas_cast->setText(b);
}
void MainWindow::resizeEvent(QResizeEvent* event){
	this->QMainWindow::resizeEvent(event);
    //manager.onResize();
	
}
MainWindow::~MainWindow(){
    q->~store();
    delete ui;
}

