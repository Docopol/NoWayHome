/*
 * helper.c
 *
 *  Created on: Oct 30, 2022
 *      Author: Dominic Olimid
 */

#include "helper.h"

int Check_Threshold(int *nb_exceeded, float *sensor_values, float *threshold, int *threshold_status)
{
	for(int i=0; i<6; i++)
	{
		if(sensor_values[i] > threshold[i] || sensor_values[i] < -threshold[i])
		{
			if(threshold_status[i] == 0)
			{
				threshold_status[i] = 1;
				nb_exceeded[0] += 1;
			}
		}
	}
	return;
}
