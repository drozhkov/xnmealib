/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: parser.h 4 2007-08-27 13:11:03Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

#ifndef __XNMEA_PARSER_H__
#define __XNMEA_PARSER_H__

#include "info.h"

#ifdef  __cplusplus
extern "C" {
#endif

/*
 * high level
 */

typedef struct _xnmeaPARSER
{
    void *top_node;
    void *end_node;
    unsigned char *buffer;
    int buff_size;
    int buff_use;

} xnmeaPARSER;

int     xnmea_parser_init(xnmeaPARSER *parser);
void    xnmea_parser_destroy(xnmeaPARSER *parser);

int     xnmea_parse(
        xnmeaPARSER *parser,
        const char *buff, int buff_sz,
        xnmeaINFO *info
        );

/*
 * low level
 */

int     xnmea_parser_push(xnmeaPARSER *parser, const char *buff, int buff_sz);
int     xnmea_parser_top(xnmeaPARSER *parser);
int     xnmea_parser_pop(xnmeaPARSER *parser, void **pack_ptr);
int     xnmea_parser_peek(xnmeaPARSER *parser, void **pack_ptr);
int     xnmea_parser_drop(xnmeaPARSER *parser);
int     xnmea_parser_buff_clear(xnmeaPARSER *parser);
int     xnmea_parser_queue_clear(xnmeaPARSER *parser);

#ifdef  __cplusplus
}
#endif

#endif /* __XNMEA_PARSER_H__ */
