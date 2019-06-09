/*
 * elapsedtime.h
 *
 *  Created on: 06.07.2018
 *      Author: Jean J. Labrosse, Silicon Labs
 *      https://www.embedded-computing.com/hardware/measuring-code-execution-time-on-arm-cortex-m-mcus
 */

/*
********************************************************************************
*                       MODULE TO MEASURE EXECUTION TIME
********************************************************************************
*/

/*
********************************************************************************
*                MAXIMUM NUMBER OF ELAPSED TIME MEASUREMENT SECTIONS
********************************************************************************
*/

#define  ELAPSED_TIME_MAX_SECTIONS  10

/*
********************************************************************************
*                             FUNCTION PROTOTYPES
********************************************************************************
*/

void  elapsed_time_clr   (uint32_t  i);      // Clear measured values
void  elapsed_time_init  (void);             // Module initialization
void  elapsed_time_start (uint32_t  i);      // Start measurement
void  elapsed_time_stop  (uint32_t  i);      // Stop  measurement
//void  elapsed_time_read	 (uint32_t	i, uint32_t *out);
/* ELAPSEDTIME_H_ */
