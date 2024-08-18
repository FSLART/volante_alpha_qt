/**
* @file mainwindow.cpp
* @brief This file contains the logic for the MainWindow class
* @see mainwindow.h
* @see store.h
* @see flabel.h
* @author João Vieira
* This piece of software was developed for the T24e project belonging to the LART Team
**/
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./store.h"
#include <cstddef>
#include <qmainwindow.h>
#include "flabel.h"

static store* store_ref;
/**
* @brief Constructor for the MainWindow Class.
*        @b Connects @b **most** variables from the store to the FLabels on the screen.
**/
MainWindow::MainWindow(QWidget *parent, QString serialDev)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){

    store_ref = new store(serialDev);

    ui->setupUi(this);
    //store_ref->setParent(this);
    //store_ref->requestSlotAttachment();
		
		FLabel* EngineTemperature_Label= this->findChild<FLabel*>("EngineTemperature_Label");
		FLabel* BatteryVoltage_Label = this->findChild<FLabel*>("BatteryVoltage_Label");
		FLabel* VehicleSpeed_Label = this->findChild<FLabel*>("VehicleSpeed_Label");
		connect(store_ref, &store::engineTemperatureChanged, EngineTemperature_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);
		connect(store_ref, &store::batteryVoltageChanged, BatteryVoltage_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
		connect(store_ref, &store::vehicleSpeedChanged, VehicleSpeed_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);
		/**
		* @brief bellow is the spaggeti code thats preprocessed to permit portable use between vehicles 
	    *        The magic occurs in qmake through the DEFINES variable.
		* @see bson_var.h
		**/
	
		#ifdef __LART_T24__
            FLabel* Soc_Label = this->findChild<FLabel*>("Soc_Label");
			FLabel* BatteryTemperature_Label = this->findChild<FLabel*>("BatteryTemperature_Label");
			FLabel* InverterTemperature_Label = this->findChild<FLabel*>("InverterTemperature_Label");
			FLabel* Power_Label = this->findChild<FLabel*>("Power_Label");
			//TODO change representation of laptime
			FLabel* LapTime_Label = this->findChild<FLabel*>("LapTime_Label");
			FLabel* DiffTime_Label = this->findChild<FLabel*>("DiffTime_Label");
			FLabel* LapCount_Label = this->findChild<FLabel*>("LapCount_Label");
			//FLabel* TyreTemperature_Label = this->findChild<FLabel*>("TyreTemperature_Label");
			connect(store_ref, &store::socChanged, Soc_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
			connect(store_ref, &store::batteryTemperatureChanged, BatteryTemperature_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
                        connect(store_ref, &store::inverterTemperatureChanged, InverterTemperature_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);
                        connect(store_ref, &store::powerChanged, Power_Label, (void (FLabel::*)(short, short))&FLabel::setVisual);
			connect(store_ref, &store::lapTimeChanged, LapTime_Label, (void (FLabel::*)(QTime, QTime))&FLabel::setVisual);
			connect(store_ref, &store::diffLapTimeChanged, DiffTime_Label, (void (FLabel::*)(QTime, QTime))&FLabel::setVisual);
                        connect(store_ref, &store::lapCountChanged, LapCount_Label, (void (FLabel::*)(short, short))&FLabel::setVisual);
			//connect(store_ref, &store::tyreTemperatureChanged, TyreTemperature_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);


		#endif
        connect(store_ref, &store::engineTemperatureChanged,EngineTemperature_Label,(void(FLabel::*)(int,int))&FLabel::setVisual);
}


/**
* @brief A getter for a store pointer. <b> use with caution, </b>
* 		 <em> with great power comes great *frickery... This function is by reference and should be used for startup stuff</em> 
* @returns a pointer to the store object
**/
store* MainWindow::getStore(){
    return store_ref;
}
/**
* @brief Destructor for the MainWindow Class.
*        @b Deletes the store object. which can cause some odd behaviour to happen
**/
MainWindow::~MainWindow(){
    store_ref->~store(); 
    delete ui;
}

