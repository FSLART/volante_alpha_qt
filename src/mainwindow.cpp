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

