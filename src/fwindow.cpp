#include "fwindow.h"

/**
* @file fwindow.cpp
* @brief This file contains the logic for the FWindow class
* @see mainwindow.h
* @see fwindow.h
* @see store.h
* @see flabel.h
* @author João Vieira
* @author Bruno Vicente
* This piece of software was developed for the T24e project belonging to the LART Team
**/

#include "./store.h"
#include <cstddef>
#include "flabel.h"

static store* store_ref;
/**
* @brief Constructor for the FWindow Class.
*        @b TODO
**/
FWindow::FWindow(QWidget *parent, QString serialDev)
    : QMainWindow(parent){

    store_ref = new store(serialDev);




}


/**
* @brief A getter for a store pointer. <b> use with caution, </b>
* 		 <em> with great power comes great *frickery... This function is by reference and should be used for startup stuff</em>
* @returns a pointer to the store object
**/
store* FWindow::getStore(){
    return store_ref;
}
/**
* @brief Destructor for the FWindow Class.
*        @b Deletes the store object. which can cause some odd behaviour to happen
**/
FWindow::~FWindow(){
    store_ref->~store();

}

