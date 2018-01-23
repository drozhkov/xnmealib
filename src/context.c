/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: context.c 17 2008-03-11 11:56:11Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

#include "xnmea/context.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

xnmeaPROPERTY * xnmea_property()
{
    static xnmeaPROPERTY prop = {
        0, 0, XNMEA_DEF_PARSEBUFF
        };

    return &prop;
}

void xnmea_trace(const char *str, ...)
{
    int size;
    va_list arg_list;
    char buff[XNMEA_DEF_PARSEBUFF];
    xnmeaTraceFunc func = xnmea_property()->trace_func;

    if(func)
    {
        va_start(arg_list, str);
        size = XNMEA_POSIX(vsnprintf)(&buff[0], XNMEA_DEF_PARSEBUFF - 1, str, arg_list);
        va_end(arg_list);

        if(size > 0)
            (*func)(&buff[0], size);
    }
}

void xnmea_trace_buff(const char *buff, int buff_size)
{
    xnmeaTraceFunc func = xnmea_property()->trace_func;
    if(func && buff_size)
        (*func)(buff, buff_size);
}

void xnmea_error(const char *str, ...)
{
    int size;
    va_list arg_list;
    char buff[XNMEA_DEF_PARSEBUFF];
    xnmeaErrorFunc func = xnmea_property()->error_func;

    if(func)
    {
        va_start(arg_list, str);
        size = XNMEA_POSIX(vsnprintf)(&buff[0], XNMEA_DEF_PARSEBUFF - 1, str, arg_list);
        va_end(arg_list);

        if(size > 0)
            (*func)(&buff[0], size);
    }
}
