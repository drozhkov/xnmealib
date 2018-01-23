/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: time.h 4 2007-08-27 13:11:03Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

/*! \file */

#ifndef __XNMEA_TIME_H__
#define __XNMEA_TIME_H__

#include "config.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * Date and time data
 * @see nmea_time_now
 */
typedef struct _xnmeaTIME
{
    int     year;       /**< Years since 1900 */
    int     mon;        /**< Months since January - [0,11] */
    int     day;        /**< Day of the month - [1,31] */
    int     hour;       /**< Hours since midnight - [0,23] */
    int     min;        /**< Minutes after the hour - [0,59] */
    int     sec;        /**< Seconds after the minute - [0,59] */
    int     hsec;       /**< Hundredth part of second - [0,99] */

} xnmeaTIME;

/**
 * \brief Get time now to xnmeaTIME structure
 */
void xnmea_time_now(xnmeaTIME *t);

#ifdef  __cplusplus
}
#endif

#endif /* __XNMEA_TIME_H__ */
