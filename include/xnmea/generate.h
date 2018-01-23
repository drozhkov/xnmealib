/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: generate.h 4 2007-08-27 13:11:03Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

#ifndef __XNMEA_GENERATE_H__
#define __XNMEA_GENERATE_H__

#include "sentence.h"

#ifdef  __cplusplus
extern "C" {
#endif

int     xnmea_generate(
        char *buff, int buff_sz,        /* buffer */
        const xnmeaINFO *info,          /* source info */
        int generate_mask,           /* mask of sentence`s (e.g. GPGGA | GPGSA) */
        xnmeaSOURCE source
        );

int     xnmea_gen_GGA(char *buff, int buff_sz, xnmeaGGA *pack, xnmeaSOURCE source);
int     xnmea_gen_GSA(char *buff, int buff_sz, xnmeaGSA *pack, xnmeaSOURCE source);
int     xnmea_gen_GSV(char *buff, int buff_sz, xnmeaGSV *pack, xnmeaSOURCE source);
int     xnmea_gen_RMC(char *buff, int buff_sz, xnmeaRMC *pack, xnmeaSOURCE source);
int     xnmea_gen_VTG(char *buff, int buff_sz, xnmeaVTG *pack, xnmeaSOURCE source);

void    xnmea_info2GGA(const xnmeaINFO *info, xnmeaGGA *pack);
void    xnmea_info2GSA(const xnmeaINFO *info, xnmeaGSA *pack);
void    xnmea_info2RMC(const xnmeaINFO *info, xnmeaRMC *pack);
void    xnmea_info2VTG(const xnmeaINFO *info, xnmeaVTG *pack);

int     xnmea_gsv_npack(int sat_count);
void    xnmea_info2GSV(const xnmeaINFO *info, xnmeaGSV *pack, int pack_idx);

#ifdef  __cplusplus
}
#endif

#endif /* __XNMEA_GENERATE_H__ */
