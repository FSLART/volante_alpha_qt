#ifndef BSON_VAR_H
#define BSON_VAR_H
//T_14 
#define BSON_RPM "rpm"
#define BSON_VEHICLESPEED "vel"
#define BSON_ENGINETEMPERATURE "eng_t"
#define BSON_BATTERYVOLTAGE "bat_v"
#ifdef __LART_T14__
	#define BSON_GEARSHIFT "gear"
	#define BSON_OILPRESSURE "oil_p"
	#define BSON_OILTEMPERATURE "oil_t"
	#define BSON_DATALOGGERSTATUS "dtl_s"
	#define BSON_AFR "af_r"
	#define BSON_TCSLIP "tc_s"
	#define BSON_TCLAUNCH "tc_l"
#endif
#ifdef __LART_T24__
	#define BSON_SOC "soc"
	#define BSON_BATTERYTEMPERATURE "bat_t"
	#define BSON_INVERTERTEMPERATURE "inv_t"
	#define BSON_POWER "pow"
	#define BSON_LAPCOUNT "lap_c"
	#define BSON_LAPTIME "lap_t"
#endif 



#endif // BSON_VAR_H
