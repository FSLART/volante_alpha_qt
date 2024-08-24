/**
* @file bson_var.h
* @brief This file contains the declaration of the BSON variables that allow flexibility and some portability between LART's various vehicles.
* @author João Vieira
* This piece of software was developed for the T24e project belonging to the LART Team
**/
#ifndef BSON_VAR_H
#define BSON_VAR_H

/**
* @brief Common Variables between both T14 and T24e
**/
#define BSON_RPM "rpm"
#define BSON_VEHICLESPEED "vel"
#define BSON_MOTORTEMPERATURE "eng_t"
#define BSON_BATTERYVOLTAGE "bat_v"
#define BSON_IGNITION "ignition"

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
    #define BSON_HV "hv"
    #define BSON_MENU "menu"
    #define BSON_POWER_LIMIT "power_available"
    #define BSON_LV_SOC "lv_soc"
#endif 



#endif // BSON_VAR_H
