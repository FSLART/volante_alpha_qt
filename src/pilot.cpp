/**
* @file pilot.cpp
* @brief This file contains the logic for the PilotWindow class
* @see pilot.h
* @see store.h
* @see flabel.h
* @author Bruno Vicente
* This piece of software was developed for the T24e project belonging to the LART Team
**/

#include "pilot.h"
#include "./ui_voidsterdebugwindow.h"

#include "./store.h"
#include <cstddef>
#include <qmainwindow.h>
#include "flabel.h"
#include "QLCDNumber"
static store* store_ref;



/**
* @brief Constructor for the VoidsterdebugWindow Class.
*        @b Connects @b **most** variables from the store to the FLabels on the screen.
**/
PilotWindow::PilotWindow(QWidget *parent, QString serialDev)
    : QMainWindow(parent)
    , ui(new Ui::~PilotWindow){

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
            QLCDNumber* SPEED_DISPLAY  = this->findChild<QLCDNumber*>("SPEED_DISPLAY");
            connect(store_ref, &store::rpmChanged, SPEED_DISPLAY, (void (QLCDNumber::*)(short, short))&QLCDNumber::setAveragedVisualChangeSec);

}




/**
* @brief A getter for a store pointer. <b> use with caution, </b>
* 		 <em> with great power comes great *frickery... This function is by reference and should be used for startup stuff</em>
* @returns a pointer to the store object
**/
store* PilotWindow::getStore(){
    return store_ref;
}
/**
* @brief Destructor for the VoidsterdebugWindow Class.
*        @b Deletes the store object. which can cause some odd behaviour to happen
**/
PilotWindow::~PilotWindow(){
    store_ref->~store();
    delete ui;
}
