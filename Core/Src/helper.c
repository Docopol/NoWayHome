/*
 * helper.c
 *
 *  Created on: Oct 30, 2022
 *      Author: Dominic Olimid
 */

#include "helper.h"

void Check_Threshold(float *sensor_values, float *threshold, int *threshold_status)
{
	for(int i=0; i<6; i++)
	{
		if(sensor_values[i] > threshold[i] || sensor_values[i] < -threshold[i])
		{
			threshold_status[i] = 1;
		}
	}
	return;
}
