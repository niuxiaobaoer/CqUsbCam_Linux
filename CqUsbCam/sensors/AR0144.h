#ifndef _AR0144_H_
#define _AR0144_H_

/****************************************************************************************\
 * This is the header file for the Caiqi usb camera for Linux/Mac						*
 *                                                                                	*
 * Author              :        nxb     											* 												
 * License             :        GPL Ver 2.0                                       	*
 * Copyright           :        Caiqi Electronics Inc.								*
 * Date written        :        Feb 27, 2018
 * Modification Notes  :                                                          	*
 *    1. nxb, Feb 27, 2018                                 						*
 *       Add documentation.                                          				*
 *                      															*
 *                                                                                	*
 \***************************************************************************************/
#include <stdio.h>
#include <string>
#include <list>

#include "../Types.h"
#include "../cyusb.h"
#include "../tagSensor.h"


//extern tagSensor sensor_AR0144

void RegisterSensor_AR0144(list<tagSensor>& sensorList);

#endif // _AR0144_H_


