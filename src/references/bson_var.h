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
#define BSON_LV_BATTERYVOLTAGE "bat_v"
#define BSON_MENU "menu"
#define BSON_IGNITION "ignition"


#ifdef __LART_T14__
#define BSON_GEARSHIFT "gear"
#define BSON_OILPRESSURE "oil_p"
#define BSON_OILTEMPERATURE "oil_t"
#define BSON_DATALOGGERSTATUS "dtl_s"
#define BSON_AFR "af_r"
#define BSON_TCSLIP "tc_s"
#define BSON_TCLAUNCH "tc_l"
#undef SIZE_OF_BSON
#define SIZE_OF_BSON 128
#endif

#ifdef __LART_T24__

#define BSON_MENU "menu"
#define BSON_SOC "soc"
#define BSON_LV_SOC "lv_soc"
#define BSON_BATTERYTEMPERATURE "bat_t"
#define BSON_INVERTERTEMPERATURE "inv_t"
#define BSON_POWER "pow"
#define BSON_LAPCOUNT "lap_c"
#define BSON_LAPTIME "lap_t"
#define BSON_INVERTERVoltage "inv_v"
#define BSON_HV_BATTERYVOLTAGE "hv_bat"
#define BSON_MAX_CELL_TEMP "max_cell_temp"
#define BSON_PAGE "page"
#define BSON_POWER_LIMIT "power_available"



#define BSON_VCU_OK "vcu_ok"
#define BSON_TCU_OK "tcu_ok"
#define BSON_DATALOGGER_OK "datalogger_ok"
#define BSON_ACU_OK "acu_ok"
#define BSON_DYNAMICS_R_OK "dynamics_r_ok"
#define BSON_DYNAMICS_F_OK "dynamics_f_ok"
#define BSON_BRAKE_LIGHT_OK "brake_light_ok"
#define BSON_ALC_OK "alc_ok"
#define BSON_INVERTER_OK "inverter_ok"
#define BSON_MAX_CELL_TEMP "max_cell_temp"
#define BSON_MIN_CELL_TEMP "min_cell_temp"
#define BSON_MIN_VOLTAGE "min_cell_v"
#define BSON_MAX_VOLTAGE "max_cell_v"
#define BSON_POP_UP_COUNT "pop_up_count"
#define BSON_POP_UP_HISTORY "pop_up_hist"
#define BSON_POP_UP_ENABLE "pop_up_en"
#define BSON_APPS "apps"
/*
    **********************************
    *******	Valores Calibração *******
    **********************************
*/
#define BSON_ASK_CALIBRATION_VALUES "calib"
#define BSON_SUSPENSAO_TRASEIRA_L "suspencao_t_l"
#define BSON_SUSPENSAO_TRASEIRA_R "suspencao_t_r"
#define BSON_SUSPENSAO_DIANTEIRA_L "suspencao_d_l"
#define BSON_SUSPENSAO_DIANTEIRA_R "suspencao_d_r"


#endif // __LART_T24__

#endif // BSON_VAR_H
