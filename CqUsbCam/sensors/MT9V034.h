#ifndef _MY9V034_H_
#define _MY9V034_H_

/****************************************************************************************\
 * This is the header file for the Caiqi usb camera for Linux/Mac						*
 *                                                                                	*
 * Author              :        nxb     											* 												
 * License             :        GPL Ver 2.0                                       	*
 * Copyright           :        Caiqi Electronics Inc.								*
 * Date written        :        nov 05, 2017                                    	*
 * Modification Notes  :                                                          	*
 *    1. nxb, nov 05, 2017                                  						*
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


void RegisterSensor_MT9V034(list<tagSensor>& sensorList);


#endif
