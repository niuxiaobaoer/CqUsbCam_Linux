#ifndef _SC130GS_H_
#define _SC130GS_H_

/****************************************************************************************\
 * This is the header file for the Caiqi usb camera for Linux/Mac						*
 *                                                                                	*
 * Author              :        nxb     											* 												
 * License             :        GPL Ver 2.0                                       	*
 * Copyright           :        Caiqi Electronics Inc.								*
 * Date written        :        Jun, 21, 2018                                    	*
 * Modification Notes  :                                                          	*
 *    1. nxb, Jun 21, 2018                                  						*
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




void RegisterSensor_SC130GS(list<tagSensor>& sensorList);

#endif // _SC130GS_H_
