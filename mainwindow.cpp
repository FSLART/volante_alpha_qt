#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./store.h"
#include <cstddef>
#include <qmainwindow.h>
#include "responsive_manager.h"
#include <QPushButton>
static responsive_manager manager;
static store* store_ref;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
		//manager = responsive_manager();
        store_ref = new store();
    	ui->setupUi(this);
        store_ref->setRpm(0);
        QPushButton *button = new QPushButton();
		button->setParent(this);
		button->setGeometry(0, 0, 100, 100);
		button->setText("test");
		button->show();
		//oon press store_ref->setRpm(2000);
		//on release store_ref->setRpm(7000);
        connect(button, &QPushButton::pressed, this, &MainWindow::test);

}
void MainWindow::test (){
    store_ref->setRpm(store_ref->getRpm()+500);
}
void MainWindow::resizeEvent(QResizeEvent* event){
    this->QMainWindow::resizeEvent(event);
	//get the new size
    //QSize size = this->size();
   //manager.on_resize(size);
	
}
//with great power comes great *frickery... This function is by reference and should be used for startup stuff
store* MainWindow::getStore(){
    return store_ref;
}
MainWindow::~MainWindow(){
    
    delete ui;
}

