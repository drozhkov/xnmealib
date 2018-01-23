/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: parse.h 4 2007-08-27 13:11:03Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

#ifndef __XNMEA_PARSE_H__
#define __XNMEA_PARSE_H__

#include "sentence.h"

#ifdef  __cplusplus
extern "C" {
#endif

int xnmea_pack_type(const char *buff, int buff_sz);
int xnmea_find_tail(const char *buff, int buff_sz, int *res_crc);

int xnmea_parse_GGA(const char *buff, int buff_sz, xnmeaGGA *pack);
int xnmea_parse_GSA(const char *buff, int buff_sz, xnmeaGSA *pack);
int xnmea_parse_GSV(const char *buff, int buff_sz, xnmeaGSV *pack);
int xnmea_parse_RMC(const char *buff, int buff_sz, xnmeaRMC *pack);
int xnmea_parse_VTG(const char *buff, int buff_sz, xnmeaVTG *pack);

void xnmea_GGA2info(xnmeaGGA *pack, xnmeaINFO *info);
void xnmea_GSA2info(xnmeaGSA *pack, xnmeaINFO *info);
void xnmea_GSV2info(xnmeaGSV *pack, xnmeaINFO *info);
void xnmea_RMC2info(xnmeaRMC *pack, xnmeaINFO *info);
void xnmea_VTG2info(xnmeaVTG *pack, xnmeaINFO *info);

#ifdef  __cplusplus
}
#endif

#endif /* __XNMEA_PARSE_H__ */
