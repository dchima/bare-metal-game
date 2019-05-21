/*
 * HPS Watchdog Reset
 * ------------------------------
 * Description: 
 * Simple #define for resetting watchdog
 *
 * Company: University of Leeds
 * Author: T Carpenter
 *
 */

#ifndef HPS_WATCHDOG_H_
#define HPS_WATCHDOG_H_

#define ResetWDT() *((volatile unsigned int *) 0xFFD0200C) = 0x76

#endif /* HPS_WATCHDOG_H_ */
