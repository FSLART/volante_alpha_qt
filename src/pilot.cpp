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
#include "./ui_pilot.h"

#include "./store.h"
#include <cstddef>
#include <qmainwindow.h>
#include "flabel.h"
#include "QLCDNumber"
#include "qprogressbar.h"
static store* store_ref;

#include "voidsterdebugwindow.h"


/**
* @brief Constructor for the VoidsterdebugWindow Class.
*        @b Connects @b **most** variables from the store to the FLabels on the screen.
**/
PilotWindow::PilotWindow(QWidget *parent, QString serialDev)
    : QMainWindow(parent)
    , ui(new Ui::pilot)  // This correctly matches the class name in the .ui file
{
    ui->setupUi(this);
    setWindowTitle("Pilotamos");


    store_ref = new store(serialDev);

    //store_ref->setParent(this);
    //store_ref->requestSlotAttachment();

        /**
        * @brief bellow is the spaggeti code thats preprocessed to permit portable use between vehicles
        *        The magic occurs in qmake through the DEFINES variable.
        * @see bson_var.h
        **/


            QLCDNumber* SPEED_DISPLAY = this->findChild<QLCDNumber*>("SPEED_DISPLAY");
            QLCDNumber* INVERTER_TEMP_DISPLAY = this->findChild<QLCDNumber*>("INVERTER_TEMP_DISPLAY");
            QLCDNumber* MOTOR_TEMP_DISPLAY = this->findChild<QLCDNumber*>("MOTOR_TEMP_DISPLAY");
            QLCDNumber* ACUMULATOR_TEMP_DISPLAY = this->findChild<QLCDNumber*>("ACUMULATOR_TEMP_DISPLAY");


            QProgressBar* HV_SOC_BAR = this->findChild<QProgressBar*>("HV_SOC_BAR");
            QProgressBar* LV_SOC_BAR = this->findChild<QProgressBar*>("LV_SOC_BAR");
            QProgressBar* POWER_SOC_BAR = this->findChild<QProgressBar*>("POWER_SOC_BAR");
            QProgressBar* CONSUMED_POWER_BAR = this->findChild<QProgressBar*>("CONSUMED_POWER_BAR");





            connect(store_ref, &store::vehicleSpeedChanged, [SPEED_DISPLAY](int m_vehicleVelocity) {
                        SPEED_DISPLAY->display(m_vehicleVelocity);});


            //connect(store_ref, &store::rpmChanged,SPEED_DISPLAY, (void (QLCDNumber::*)(short, short))&QLCDNumber::show;
            connect(store_ref, &store::vehicleSpeedChanged, INVERTER_TEMP_DISPLAY, QOverload<int>::of(&QLCDNumber::display));
            INVERTER_TEMP_DISPLAY->show();


            connect(store_ref, &store::vehicleSpeedChanged, MOTOR_TEMP_DISPLAY, QOverload<int>::of(&QLCDNumber::display));
            MOTOR_TEMP_DISPLAY->show();

            connect(store_ref, &store::vehicleSpeedChanged, ACUMULATOR_TEMP_DISPLAY, QOverload<int>::of(&QLCDNumber::display));
            INVERTER_TEMP_DISPLAY->show();





            connect(store_ref,&store::socChanged,[HV_SOC_BAR](int soc){
               HV_SOC_BAR->setValue(soc);
            });

            connect(store_ref,&store::socChanged,[LV_SOC_BAR](int soc){
               LV_SOC_BAR->setValue(soc);
            });

            connect(store_ref,&store::socChanged,[POWER_SOC_BAR](int soc){
               POWER_SOC_BAR->setValue(soc);
            });

            connect(store_ref,&store::socChanged,[CONSUMED_POWER_BAR](int soc){
               CONSUMED_POWER_BAR->setValue(soc);
            });
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


