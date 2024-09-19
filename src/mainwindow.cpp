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
#include "qprogressbar.h"

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
            FLabel* VehicleSpeed_Label = this->findChild<FLabel*>("Speed_Label");
            connect(store_ref, &store::vehicleSpeedChanged, VehicleSpeed_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);
		/**
		* @brief bellow is the spaggeti code thats preprocessed to permit portable use between vehicles 
	    *        The magic occurs in qmake through the DEFINES variable.
		* @see bson_var.h
		**/
	
		#ifdef __LART_T24__
            QProgressBar* Soc_Bar= this->findChild<QProgressBar*>("SOCLimitBar");
            QProgressBar* Apps_bar = this->findChild<QProgressBar*>("AppsBar");
            QProgressBar* Power_Consumed = this->findChild<QProgressBar*>("ConsumedPowerBar");
            setStyleSheets();
            FLabel* PowerLimit_Label = this->findChild<FLabel*>("PowerLimit_Label");
			//TODO change representation of laptime
                FLabel* LapTime_Label = this->findChild<FLabel*>("AbsoluteTime_Label");
                FLabel* HV_voltage_label = this->findChild<FLabel*>("HV_voltage_label");
                FLabel* SOC_label = this->findChild<FLabel*>("SOC_label");
                FLabel* TempAcc_Label = this->findChild<FLabel*>("TempAcc_Label");
                FLabel* TempMotor_Label = this->findChild<FLabel*>("TempMotor_Label");


                FLabel* POWER_label = this->findChild<FLabel*>("POWER_label");

			//FLabel* TyreTemperature_Label = this->findChild<FLabel*>("TyreTemperature_Label");
            connect(store_ref,&store::socChanged,[Soc_Bar](int soc){
               Soc_Bar->setValue(soc);
            });
            connect(store_ref,&store::powerChanged,[Power_Consumed](short power){
               Power_Consumed->setValue(power);
            });
            connect(store_ref,&store::appsChanged,[Apps_bar](int appsChanged){
                Apps_bar->setValue(appsChanged);
            });
            connect(store_ref, &store::vehicleSpeedChanged, VehicleSpeed_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);
            connect(store_ref, &store::socChanged, SOC_label, (void (FLabel::*)(int, int))&FLabel::setVisual);
            connect(store_ref,&store::bat_voltageChanged,HV_voltage_label,(void (FLabel::*)(short,short))&FLabel::setVisual);
            connect(store_ref, &store::power_limitChanged, PowerLimit_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);
            connect(store_ref, &store::powerChanged, POWER_label, (void (FLabel::*)(short, short))&FLabel::setVisual);
            connect(store_ref, &store::lapTimeChanged, LapTime_Label, (void (FLabel::*)(QTime, QTime))&FLabel::setVisual);
            connect(store_ref, &store::motorTemperatureChanged,TempMotor_Label,(void (FLabel::*)(int,int))&FLabel::setVisual);
            connect(store_ref, &store::batteryTemperatureChanged,TempAcc_Label,(void(FLabel::*)(int,int))&FLabel::setVisual);
            //
            //connect(store_ref, &store::diffLapTimeChanged, DiffTime_Label, (void (FLabel::*)(QTime, QTime))&FLabel::setVisual);
            //connect(store_ref, &store::absoluteTimeChanged, AbsoluteTime_Label, (void (FLabel::*)(QTime, QTime))&FLabel::setVisual);
            //connect(store_ref, &store::lapCountChanged, LapCount_Label, (void (FLabel::*)(short, short))&FLabel::setVisual);


		#endif
        //connect(store_ref, &store::engineTemperatureChanged,EngineTemperature_Label,(void(FLabel::*)(int,int))&FLabel::setVisual);
}

inline void MainWindow::setStyleSheets(){
    QProgressBar* Soc_Bar= this->findChild<QProgressBar*>("SOCLimitBar");
    QProgressBar* Apps_bar = this->findChild<QProgressBar*>("AppsBar");
    QProgressBar* Power_Consumed = this->findChild<QProgressBar*>("ConsumedPowerBar");
    QString k =QString("QProgressBar{color: %1; background: %2}QProgressBar::chunk{background: %3}").arg(this->palette().background().color().name(), this->palette().background().color().name(), this->palette().background().color().name());
    //Apps_bar->setStyleSheet(k);
    //Soc_Bar->setStyleSheet(k);


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

