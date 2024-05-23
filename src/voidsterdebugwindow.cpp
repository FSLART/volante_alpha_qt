/**
* @file mainwindow.cpp
* @brief This file contains the logic for the MainWindow class
* @see mainwindow.h
* @see store.h
* @see flabel.h
* @author João Vieira
* This piece of software was developed for the T24e project belonging to the LART Team
**/

        #include "voidsterdebugwindow.h"
        #include "./ui_voidsterdebugwindow.h"

#include "./store.h"
#include <cstddef>
#include <qmainwindow.h>
#include "flabel.h"

static store* store_ref;
/**
* @brief Constructor for the MainWindow Class.
*        @b Connects @b **most** variables from the store to the FLabels on the screen.
**/

VoidsterdebugWindow::VoidsterdebugWindow(QWidget *parent, QString serialDev)
    : QMainWindow(parent)
    , ui(new Ui::VoidsterdebugWindow){

    store_ref = new store(serialDev);

    ui->setupUi(this);
    //store_ref->setParent(this);
    //store_ref->requestSlotAttachment();
		
		/**
		* @brief bellow is the spaggeti code thats preprocessed to permit portable use between vehicles 
	    *        The magic occurs in qmake through the DEFINES variable.
		* @see bson_var.h
		**/
		#ifdef __LART_T24__
		#ifdef __LART_DEBUG__

		#endif

		#endif
		
}


/**
* @brief A getter for a store pointer. <b> use with caution, </b>
* 		 <em> with great power comes great *frickery... This function is by reference and should be used for startup stuff</em> 
* @returns a pointer to the store object
**/
store* VoidsterdebugWindow::getStore(){
    return store_ref;
}
/**
* @brief Destructor for the MainWindow Class.
*        @b Deletes the store object. which can cause some odd behaviour to happen
**/
VoidsterdebugWindow::~VoidsterdebugWindow(){
    store_ref->~store(); 
    delete ui;
}
