/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: tok.h 4 2007-08-27 13:11:03Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

#ifndef __XNMEA_TOK_H__
#define __XNMEA_TOK_H__

#include "config.h"

#ifdef  __cplusplus
extern "C" {
#endif

int     xnmea_calc_crc(const char *buff, int buff_sz);
int     xnmea_atoi(const char *str, int str_sz, int radix);
double  xnmea_atof(const char *str, int str_sz);
int     xnmea_printf(char *buff, int buff_sz, const char *format, ...);
int     xnmea_scanf(const char *buff, int buff_sz, const char *format, ...);

#ifdef  __cplusplus
}
#endif

#endif /* __XNMEA_TOK_H__ */
