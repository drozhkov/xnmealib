/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: info.c 17 2008-03-11 11:56:11Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

#include <string.h>

#include "xnmea/info.h"

void xnmea_zero_INFO(xnmeaINFO *info)
{
    memset(info, 0, sizeof(xnmeaINFO));
    xnmea_time_now(&info->utc);
    info->sig = XNMEA_SIG_BAD;
    info->fix = XNMEA_FIX_BAD;
}
