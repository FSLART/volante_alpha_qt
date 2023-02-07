#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./store.h"
#include <cstddef>
#include <qmainwindow.h>
#include "responsive_manager.h"
#include "resp_label.h"
static responsive_manager manager;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
		manager = responsive_manager();
    	ui->setupUi(this);
        //q = new store();
		
		//qDebug()<< q->awaitNewPacket();

		
		
        //grab the QObject which name is "label_mudancas"
        QObject *label_mudancas = this->findChild<QObject*>("label_mudancas");
        //cast it to a QLabel
        QLabel *label_mudancas_cast = qobject_cast<QLabel*>(label_mudancas);
       auto hookMudanças = new resp_label(false, this->size());
	   
       hookMudanças->ref=label_mudancas_cast;
       manager.add_hook(hookMudanças);

}
void MainWindow::resizeEvent(QResizeEvent* event){
	this->QMainWindow::resizeEvent(event);
	//get the new size
	QSize size = this->size();
    manager.on_resize(size);
	
}
MainWindow::~MainWindow(){
    q->~store();
    delete ui;
}

