/*
 * sensorIO.c
 *
 *  Created on: 28 Oct 2022
 *      Author: Dominic Olimid
 */

#include "sensorIO.h"

void Read_Acc(float *accel_data)
{
	int16_t accel_data_i16[3] = { 0 };
	BSP_ACCELERO_AccGetXYZ(accel_data_i16);

	accel_data[0] = (float)accel_data_i16[0] / 100.0f;
	accel_data[1] = (float)accel_data_i16[1] / 100.0f;
	accel_data[2] = (float)accel_data_i16[2] / 100.0f;

	return;
}
void Read_Temp(float *temp_data)
{
	temp_data[0] = BSP_TSENSOR_ReadTemp();			// read temperature sensor
	return;
}

