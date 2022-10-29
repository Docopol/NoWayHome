/*
 * sensorIO.h
 *
 *  Created on: 28 Oct 2022
 *      Author: Dominic Olimid
 */

#ifndef SRC_SENSORIO_H_
#define SRC_SENSORIO_H_

#include "../../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_accelero.h"
#include "../../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_tsensor.h"
#include "../../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_magneto.h"
#include "../../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_gyro.h"
#include "../../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_hsensor.h"
#include "../../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01_psensor.h"
#include "../../Drivers/BSP/B-L475E-IOT01/stm32l475e_iot01.h"

void Read_Acc(float *accel_data);
void Read_Temp(float *temp_data);
void Read_Mag(float *mag_data);
void Read_Gyro(float *gyro_data);
void Read_Hum(float *hum_data);
void Read_Pres(float *pres_data);

#endif /* SRC_SENSORIO_H_ */



