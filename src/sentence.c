/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: sentence.c 17 2008-03-11 11:56:11Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

#include "xnmea/sentence.h"

#include <string.h>

void xnmea_zero_GGA(xnmeaGGA *pack)
{
    memset(pack, 0, sizeof(xnmeaGGA));
    xnmea_time_now(&pack->utc);
    pack->ns = 'N';
    pack->ew = 'E';
    pack->elv_units = 'M';
    pack->diff_units = 'M';
}

void xnmea_zero_GSA(xnmeaGSA *pack)
{
    memset(pack, 0, sizeof(xnmeaGSA));
    pack->fix_mode = 'A';
    pack->fix_type = XNMEA_FIX_BAD;
}

void xnmea_zero_GSV(xnmeaGSV *pack)
{
    memset(pack, 0, sizeof(xnmeaGSV));
}

void xnmea_zero_RMC(xnmeaRMC *pack)
{
    memset(pack, 0, sizeof(xnmeaRMC));
    xnmea_time_now(&pack->utc);
    pack->status = 'V';
    pack->ns = 'N';
    pack->ew = 'E';
    pack->declin_ew = 'E';
}

void xnmea_zero_VTG(xnmeaVTG *pack)
{
    memset(pack, 0, sizeof(xnmeaVTG));
    pack->dir_t = 'T';
    pack->dec_m = 'M';
    pack->spn_n = 'N';
    pack->spk_k = 'K';
}
