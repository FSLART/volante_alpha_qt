#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./store.h"
#include <cstddef>
#include <qmainwindow.h>
#include "flabel.h"

static store* store_ref;

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

		#ifdef __LART_T14__
			FLabel* OilPressure_Label = this->findChild<FLabel*>("OilPressure_Label");
			FLabel* OilTemperature_Label = this->findChild<FLabel*>("OilTemperature_Label");
			FLabel* DataLogger_Label = this->findChild<FLabel*>("DataLogger_Label");
			FLabel* Lambda_Label = this->findChild<FLabel*>("Lambda_Label");
			FLabel* TcSlip_Label = this->findChild<FLabel*>("TcSlip_Label");
			FLabel* TcLaunch_Label = this->findChild<FLabel*>("TcLaunch_Label");

			connect(store_ref, &store::dataLoggerChanged, DataLogger_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);
			connect(store_ref, &store::lambdaChanged, Lambda_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
			connect(store_ref, &store::tcSlipChanged, TcSlip_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);
			connect(store_ref, &store::tcLaunchChanged, TcLaunch_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);

			connect(store_ref, &store::oilPressureChanged, OilPressure_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
			connect(store_ref, &store::oilTemperatureChanged, OilTemperature_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
		#endif
		#ifdef __LART_T24__
			FLabel* Soc_Label = this->findChild<FLabel*>("Soc_Label");
			FLabel* BatteryTemperature_Label = this->findChild<FLabel*>("BatteryTemperature_Label");
			FLabel* InverterTemperature_Label = this->findChild<FLabel*>("InverterTemperature_Label");
			FLabel* Power_Label = this->findChild<FLabel*>("Power_Label");
			//TODO change representation of laptime
			FLabel* LapTime_Label = this->findChild<FLabel*>("LapTime_Label");
			FLabel* LapCount_Label = this->findChild<FLabel*>("LapCount_Label");
			//FLabel* TyreTemperature_Label = this->findChild<FLabel*>("TyreTemperature_Label");
			connect(store_ref, &store::socChanged, Soc_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
			connect(store_ref, &store::batteryTemperatureChanged, BatteryTemperature_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
			connect(store_ref, &store::inverterTemperatureChanged, InverterTemperature_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
			connect(store_ref, &store::powerChanged, Power_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
			connect(store_ref, &store::lapTimeChanged, LapTime_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);
			connect(store_ref, &store::lapCountChanged, LapCount_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);
			//connect(store_ref, &store::tyreTemperatureChanged, TyreTemperature_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);


		#endif
		
}


//with great power comes great *frickery... This function is by reference and should be used for startup stuff
store* MainWindow::getStore(){
    return store_ref;
}
MainWindow::~MainWindow(){
    store_ref->~store(); 
    delete ui;
}

