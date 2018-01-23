/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: units.h 4 2007-08-27 13:11:03Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

#ifndef __XNMEA_UNITS_H__
#define __XNMEA_UNITS_H__

#include "config.h"

/*
 * Distance units
 */

#define XNMEA_TUD_YARDS      (1.0936)        /**< Yeards, meter * XNMEA_TUD_YARDS = yard */
#define XNMEA_TUD_KNOTS      (1.852)         /**< Knots, kilometer / XNMEA_TUD_KNOTS = knot */
#define XNMEA_TUD_MILES      (1.609)         /**< Miles, kilometer / XNMEA_TUD_MILES = mile */

/*
 * Speed units
 */

#define XNMEA_TUS_MS         (3.6)           /**< Meters per seconds, (k/h) / XNMEA_TUS_MS= (m/s) */

#endif /* __XNMEA_UNITS_H__ */
