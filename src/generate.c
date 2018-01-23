/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: generate.c 17 2008-03-11 11:56:11Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

#include "xnmea/tok.h"
#include "xnmea/sentence.h"
#include "xnmea/generate.h"
#include "xnmea/units.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

static const char * xnmea_source_name(xnmeaSOURCE source)
{
    switch (source) {
        case XNMEA_SOURCE_GALILEO:
            return "GA";

        case XNMEA_SOURCE_GLONASS:
            return "GL";

        case XNMEA_SOURCE_GPS:
            return "GP";

        case XNMEA_SOURCE_GLONASS_GPS:
            return "GN";
    }

    return "??";
}

int xnmea_gen_GGA(char *buff, int buff_sz, xnmeaGGA *pack, xnmeaSOURCE source)
{
    return xnmea_printf(buff, buff_sz,
        "$%sGGA,%02d%02d%02d.%02d,%07.4f,%C,%07.4f,%C,%1d,%02d,%03.1f,%03.1f,%C,%03.1f,%C,%03.1f,%04d",
        xnmea_source_name(source),
        pack->utc.hour, pack->utc.min, pack->utc.sec, pack->utc.hsec,
        pack->lat, pack->ns, pack->lon, pack->ew,
        pack->sig, pack->satinuse, pack->HDOP, pack->elv, pack->elv_units,
        pack->diff, pack->diff_units, pack->dgps_age, pack->dgps_sid);
}

int xnmea_gen_GSA(char *buff, int buff_sz, xnmeaGSA *pack, xnmeaSOURCE source)
{
    return xnmea_printf(buff, buff_sz,
        "$%sGSA,%C,%1d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%03.1f,%03.1f,%03.1f",
        xnmea_source_name(source),
        pack->fix_mode, pack->fix_type,
        pack->sat_prn[0], pack->sat_prn[1], pack->sat_prn[2], pack->sat_prn[3], pack->sat_prn[4], pack->sat_prn[5],
        pack->sat_prn[6], pack->sat_prn[7], pack->sat_prn[8], pack->sat_prn[9], pack->sat_prn[10], pack->sat_prn[11],
        pack->PDOP, pack->HDOP, pack->VDOP);
}

int xnmea_gen_GSV(char *buff, int buff_sz, xnmeaGSV *pack, xnmeaSOURCE source)
{
    return xnmea_printf(buff, buff_sz,
        "$%sGSV,%1d,%1d,%02d,"
        "%02d,%02d,%03d,%02d,"
        "%02d,%02d,%03d,%02d,"
        "%02d,%02d,%03d,%02d,"
        "%02d,%02d,%03d,%02d",
        xnmea_source_name(source),
        pack->pack_count, pack->pack_index + 1, pack->sat_count,
        pack->sat_data[0].id, pack->sat_data[0].elv, pack->sat_data[0].azimuth, pack->sat_data[0].sig,
        pack->sat_data[1].id, pack->sat_data[1].elv, pack->sat_data[1].azimuth, pack->sat_data[1].sig,
        pack->sat_data[2].id, pack->sat_data[2].elv, pack->sat_data[2].azimuth, pack->sat_data[2].sig,
        pack->sat_data[3].id, pack->sat_data[3].elv, pack->sat_data[3].azimuth, pack->sat_data[3].sig);
}

int xnmea_gen_RMC(char *buff, int buff_sz, xnmeaRMC *pack, xnmeaSOURCE source)
{
    return xnmea_printf(buff, buff_sz,
        "$%sRMC,%02d%02d%02d.%02d,%C,%07.4f,%C,%07.4f,%C,%03.1f,%03.1f,%02d%02d%02d,%03.1f,%C,%C",
        xnmea_source_name(source),
        pack->utc.hour, pack->utc.min, pack->utc.sec, pack->utc.hsec,
        pack->status, pack->lat, pack->ns, pack->lon, pack->ew,
        pack->speed, pack->direction,
        pack->utc.day, pack->utc.mon + 1, pack->utc.year - 100,
        pack->declination, pack->declin_ew, pack->mode);
}

int xnmea_gen_VTG(char *buff, int buff_sz, xnmeaVTG *pack, xnmeaSOURCE source)
{
    return xnmea_printf(buff, buff_sz,
        "$%sVTG,%.1f,%C,%.1f,%C,%.1f,%C,%.1f,%C",
        xnmea_source_name(source),
        pack->dir, pack->dir_t,
        pack->dec, pack->dec_m,
        pack->spn, pack->spn_n,
        pack->spk, pack->spk_k);
}

void xnmea_info2GGA(const xnmeaINFO *info, xnmeaGGA *pack)
{
    xnmea_zero_GGA(pack);

    pack->utc = info->utc;
    pack->lat = fabs(info->lat);
    pack->ns = ((info->lat > 0)?'N':'S');
    pack->lon = fabs(info->lon);
    pack->ew = ((info->lon > 0)?'E':'W');
    pack->sig = info->sig;
    pack->satinuse = info->satinfo.inuse;
    pack->HDOP = info->HDOP;
    pack->elv = info->elv;
}

void xnmea_info2GSA(const xnmeaINFO *info, xnmeaGSA *pack)
{
    int it;

    xnmea_zero_GSA(pack);

    pack->fix_type = info->fix;
    pack->PDOP = info->PDOP;
    pack->HDOP = info->HDOP;
    pack->VDOP = info->VDOP;

    for(it = 0; it < XNMEA_MAXSAT; ++it)
    {
        pack->sat_prn[it] =
            ((info->satinfo.sat[it].in_use)?info->satinfo.sat[it].id:0);
    }
}

int xnmea_gsv_npack(int sat_count)
{
    int pack_count = (int)ceil(((double)sat_count) / XNMEA_SATINPACK);

    if(0 == pack_count)
        pack_count = 1;

    return pack_count;
}

void xnmea_info2GSV(const xnmeaINFO *info, xnmeaGSV *pack, int pack_idx)
{
    int sit, pit;

    xnmea_zero_GSV(pack);

    pack->sat_count = (info->satinfo.inview <= XNMEA_MAXSAT)?info->satinfo.inview:XNMEA_MAXSAT;
    pack->pack_count = xnmea_gsv_npack(pack->sat_count);

    if(pack->pack_count == 0)
        pack->pack_count = 1;

    if(pack_idx >= pack->pack_count)
        pack->pack_index = pack_idx % pack->pack_count;
    else
        pack->pack_index = pack_idx;

    for(pit = 0, sit = pack->pack_index * XNMEA_SATINPACK; pit < XNMEA_SATINPACK; ++pit, ++sit)
        pack->sat_data[pit] = info->satinfo.sat[sit];
}

void xnmea_info2RMC(const xnmeaINFO *info, xnmeaRMC *pack)
{
    xnmea_zero_RMC(pack);

    pack->utc = info->utc;
    pack->status = ((info->sig > 0)?'A':'V');
    pack->lat = fabs(info->lat);
    pack->ns = ((info->lat > 0)?'N':'S');
    pack->lon = fabs(info->lon);
    pack->ew = ((info->lon > 0)?'E':'W');
    pack->speed = info->speed / XNMEA_TUD_KNOTS;
    pack->direction = info->direction;
    pack->declination = info->declination;
    pack->declin_ew = 'E';
    pack->mode = ((info->sig > 0)?'A':'N');
}

void xnmea_info2VTG(const xnmeaINFO *info, xnmeaVTG *pack)
{
    xnmea_zero_VTG(pack);

    pack->dir = info->direction;
    pack->dec = info->declination;
    pack->spn = info->speed / XNMEA_TUD_KNOTS;
    pack->spk = info->speed;
}

int xnmea_generate(
    char *buff, int buff_sz,
    const xnmeaINFO *info,
    int generate_mask,
    xnmeaSOURCE source
    )
{
    int gen_count = 0, gsv_it, gsv_count;
    int pack_mask = generate_mask;

    xnmeaGGA gga;
    xnmeaGSA gsa;
    xnmeaGSV gsv;
    xnmeaRMC rmc;
    xnmeaVTG vtg;

    if(!buff)
        return 0;

    while(pack_mask)
    {
        if(pack_mask & GGA)
        {
            xnmea_info2GGA(info, &gga);
            gen_count += xnmea_gen_GGA(buff + gen_count, buff_sz - gen_count, &gga, source);
            pack_mask &= ~GGA;
        }
        else if(pack_mask & GSA)
        {
            xnmea_info2GSA(info, &gsa);
            gen_count += xnmea_gen_GSA(buff + gen_count, buff_sz - gen_count, &gsa, source);
            pack_mask &= ~GSA;
        }
        else if(pack_mask & GSV)
        {
            gsv_count = xnmea_gsv_npack(info->satinfo.inview);
            for(gsv_it = 0; gsv_it < gsv_count && buff_sz - gen_count > 0; ++gsv_it)
            {
                xnmea_info2GSV(info, &gsv, gsv_it);
                gen_count += xnmea_gen_GSV(buff + gen_count, buff_sz - gen_count, &gsv, source);
            }
            pack_mask &= ~GSV;
        }
        else if(pack_mask & RMC)
        {
            xnmea_info2RMC(info, &rmc);
            gen_count += xnmea_gen_RMC(buff + gen_count, buff_sz - gen_count, &rmc, source);
            pack_mask &= ~RMC;
        }
        else if(pack_mask & VTG)
        {
            xnmea_info2VTG(info, &vtg);
            gen_count += xnmea_gen_VTG(buff + gen_count, buff_sz - gen_count, &vtg, source);
            pack_mask &= ~VTG;
        }
        else
            break;

        if(buff_sz - gen_count <= 0)
            break;
    }

    return gen_count;
}
