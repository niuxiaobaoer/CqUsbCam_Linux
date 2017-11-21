#ifndef _TYPES_H_
#define _TYPES_H_

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

 typedef unsigned long cq_uint64_t;
 typedef unsigned int cq_uint32_t;
 typedef unsigned short cq_uint16_t;
 typedef unsigned char cq_uint8_t;
 
 typedef long cq_int64_t;
 typedef int cq_int32_t;
 typedef short cq_int16_t;
 typedef char cq_int8_t;
 
 typedef char cq_byte_t;
 
 typedef bool cq_bool_t;

 typedef void (*callback_t)(void *);


 
#endif // _TYPES_H_
