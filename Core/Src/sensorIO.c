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

void Read_Mag(float *mag_data)
{
	int16_t magneto_data_i16[3] = { 0 };
	BSP_MAGNETO_GetXYZ(magneto_data_i16);
	mag_data[0] = (float)magneto_data_i16[0] / 100.0f;
	mag_data[1] = (float)magneto_data_i16[1] / 100.0f;
	mag_data[2] = (float)magneto_data_i16[2] / 100.0f;

	return;
}

void Read_Gyro(float *gyro_data)
{
	int16_t gyro_data_i16[3] = { 0 };
	BSP_GYRO_GetXYZ(gyro_data_i16);
	gyro_data[0] = (float)gyro_data_i16[0] / 100.0f;
	gyro_data[1] = (float)gyro_data_i16[1] / 100.0f;
	gyro_data[2] = (float)gyro_data_i16[2] / 100.0f;

	return;
}

void Read_Hum(float *hum_data)
{
	hum_data[0] = BSP_HSENSOR_ReadHumidity();
}

void Read_Pres(float *pres_data)
{
	pres_data[0] = BSP_PSENSOR_ReadPressure();
}
