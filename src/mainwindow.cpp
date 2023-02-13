#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./store.h"
#include <cstddef>
#include <qmainwindow.h>

static store* store_ref;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){

        store_ref = new store();
    	ui->setupUi(this);

}

//with great power comes great *frickery... This function is by reference and should be used for startup stuff
store* MainWindow::getStore(){
    return store_ref;
}
MainWindow::~MainWindow(){
    
    delete ui;
}

