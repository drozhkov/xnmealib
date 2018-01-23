/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: context.h 4 2007-08-27 13:11:03Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

#ifndef __XNMEA_CONTEXT_H__
#define __XNMEA_CONTEXT_H__

#include "config.h"

#define XNMEA_DEF_PARSEBUFF  (1024)
#define XNMEA_MIN_PARSEBUFF  (256)

#ifdef  __cplusplus
extern "C" {
#endif

typedef void (*xnmeaTraceFunc)(const char *str, int str_size);
typedef void (*xnmeaErrorFunc)(const char *str, int str_size);

typedef struct _xnmeaPROPERTY
{
    xnmeaTraceFunc   trace_func;
    xnmeaErrorFunc   error_func;
    int             parse_buff_size;

} xnmeaPROPERTY;

xnmeaPROPERTY * xnmea_property();

void xnmea_trace(const char *str, ...);
void xnmea_trace_buff(const char *buff, int buff_size);
void xnmea_error(const char *str, ...);

#ifdef  __cplusplus
}
#endif

#endif /* __XNMEA_CONTEXT_H__ */
