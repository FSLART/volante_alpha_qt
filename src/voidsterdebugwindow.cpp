/**
* @file voidsterdebugwindow.cpp
* @brief This file contains the logic for the VoidsterDebugWindow class
* @see voidsterdebugwindow.h
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
#include "pilot.h"
static store* store_ref;

/**
* @brief Constructor for the VoidsterdebugWindow Class.
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

            FLabel* TotalPowerDraw_Label  = this->findChild<FLabel*>("TotalPowerDraw_Label");
            FLabel* InverterVoltage_Label = this->findChild<FLabel*>("InverterVoltage_Label");
            FLabel* Accumulator_Voltage_Label = this->findChild<FLabel*>("Accumulator_Voltage");
            FLabel* Max_cell_temp_label = this->findChild<FLabel*>("max_cell_temp");
            FLabel* SOC_Label             = this->findChild<FLabel*>("SOC_Label");
            FLabel* DTConstraints_Label   = this->findChild<FLabel*>("DTConstraints_Label");

            FLabel* ShutdownCircuit_label = this->findChild<FLabel*>("ShutdownCircuit_label");

            FLabel* VcuStatePower_Label   = this->findChild<FLabel*>("VcuStatePower_Label");
            FLabel* VcuState_label        = this->findChild<FLabel*>("VcuState_label");

            FLabel* TcuStatePower_Label   = this->findChild<FLabel*>("TcuStatePower_Label");
            FLabel* TcuState_Label        = this->findChild<FLabel*>("TcuState_Label"); 

            FLabel* Datalogger_Label      = this->findChild<FLabel*>("Datalogger_Label");

            FLabel* TelemetryValue_Label  = this->findChild<FLabel*>("TelemetryValue_Label");
            FLabel* Telemetry_Label       = this->findChild<FLabel*>("Telemetry_Label");
            ContaRotacoes * ContaRotacoes_Comp = this->findChild<ContaRotacoes*>("contaRotacoes");

            FLabel* Driverless_Label      = this->findChild<FLabel*>("Driverless_Label");
            connect(store_ref, &store::powerChanged, TotalPowerDraw_Label, (void (FLabel::*)(short, short))&FLabel::setAveragedVisualChangeSec);
            connect(store_ref,&store::rpmChanged, ContaRotacoes_Comp, &ContaRotacoes::handleChangedValue);
            connect(store_ref, &store::socChanged, SOC_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
            //connect(store_ref, &store::batteryTemperatureChanged, )
            connect(store_ref, &store::hvChanged,InverterVoltage_Label, (void (FLabel::*)(short, short))&FLabel::setVisual );
            connect(store_ref, &store::bat_voltageChanged,Accumulator_Voltage_Label, (void (FLabel::*)(short, short))&FLabel::setVisual );
            connect(store_ref, &store::max_cell_tempChanged,Max_cell_temp_label, (void (FLabel::*)(short, short))&FLabel::setVisual );
            //QObject::connect(store_ref,&store::menuChanged,this,&VoidsterdebugWindow::handleValueChanged);

            //connect(store_ref, &store::rpmChanged,VcuState_label,(void(FLabel::*)(short,short))&FLabel::setVisual);
            /*TotalPowerDraw_Label->setText("Isto é um teste 1");
            InverterVoltage_Label->setText("Isto é um teste 1");
            SOC_Label->setText("Isto é um teste 3");
            DTConstraints_Label->setText("Isto é um teste 4");

            ShutdownCircuit_label->setText("S");

            VcuStatePower_Label->setText("VP");
            VcuState_label->setText("VL");

            TcuStatePower_Label->setText("TP");
            TcuState_Label->setText("TL");
            
            Datalogger_Label->setText("DL");

            TelemetryValue_Label->setText("TV");
            Telemetry_Label->setText("TL");

            Driverless_Label->setText("DL");
*/
            

		#endif
		#endif
    //  store_ref->setHV(10);
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
* @brief Destructor for the VoidsterdebugWindow Class.
*        @b Deletes the store object. which can cause some odd behaviour to happen
**/
VoidsterdebugWindow::~VoidsterdebugWindow(){
    //store_ref->~store();
    delete store_ref;
    delete ui;
}

/*
void VoidsterdebugWindow::handleValueChanged(int newValue) {
    // Close the current window
    if(store_ref->getMenu() == 0)
    {

        delete store_ref;
        PilotWindow  *pl = new PilotWindow();
        this->close();
        pl->show();

    }

    }

*/
