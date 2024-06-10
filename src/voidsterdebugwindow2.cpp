/**
* @file voidsterdebugwindow2.cpp
* @brief This file contains the logic for the VoidsterDebugWindow2 class
* @see voidsterdebugwindow2.h
* @see store.h
* @see flabel.h
* @author João Vieira
* This piece of software was developed for the T24e project belonging to the LART Team
**/

#include "voidsterdebugwindow2.h"
#include "ui_voidsterdebugwindow2.h"

#include "./store.h"
#include <cstddef>
#include <qmainwindow.h>
#include "flabel.h"

static store* store_ref;

/**
* @brief Constructor for the VoidsterdebugWindow2 Class.
*        @b Connects @b **most** variables from the store to the FLabels on the screen.
**/
VoidsterdebugWindow2::VoidsterdebugWindow2(QWidget *parent, QString serialDev) :
    QMainWindow(parent),
    ui(new Ui::VoidsterdebugWindow2)
{
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

            //FLabel* Label  = this->findChild<FLabel*>("Label");
            //Label->setText("kvenwsnskne");

        #endif
        #endif
}


/**
* @brief A getter for a store pointer. <b> use with caution, </b>
* 		 <em> with great power comes great *frickery... This function is by reference and should be used for startup stuff</em>
* @returns a pointer to the store object
**/
store* VoidsterdebugWindow2::getStore(){
    return store_ref;
}
/**
* @brief Destructor for the VoidsterdebugWindow2 Class.
*        @b Deletes the store object. which can cause some odd behaviour to happen
**/
VoidsterdebugWindow2::~VoidsterdebugWindow2()
{
    store_ref->~store();
    delete ui;
}
