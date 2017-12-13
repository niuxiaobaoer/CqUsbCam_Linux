#ifndef _AR0135_H_
#define _AR0135_H_

/****************************************************************************************\
 * This is the header file for the Caiqi usb camera for Linux/Mac						*
 *                                                                                	*
 * Author              :        nxb     											* 												
 * License             :        GPL Ver 2.0                                       	*
 * Copyright           :        Caiqi Electronics Inc.								*
 * Date written        :        Oct 12, 2017                                    	*
 * Modification Notes  :                                                          	*
 *    1. nxb, Oct 12, 2017                                  						*
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


//extern tagSensor sensor_AR0135;

void RegisterSensor_AR0135(list<tagSensor>& sensorList);

#endif // _AR0135_H_
