/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: parse.c 17 2008-03-11 11:56:11Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

/**
 * \file parse.h
 * \brief Functions of a low level for analysis of
 * packages of NMEA stream.
 *
 * \code
 * ...
 * ptype = nmea_pack_type(
 *     (const char *)parser->buffer + nparsed + 1,
 *     parser->buff_use - nparsed - 1);
 *
 * if(0 == (node = malloc(sizeof(nmeaParserNODE))))
 *     goto mem_fail;
 *
 * node->pack = 0;
 *
 * switch(ptype)
 * {
 * case GPGGA:
 *     if(0 == (node->pack = malloc(sizeof(nmeaGPGGA))))
 *         goto mem_fail;
 *     node->packType = GPGGA;
 *     if(!nmea_parse_GPGGA(
 *         (const char *)parser->buffer + nparsed,
 *         sen_sz, (nmeaGPGGA *)node->pack))
 *     {
 *         free(node);
 *         node = 0;
 *     }
 *     break;
 * case GPGSA:
 *     if(0 == (node->pack = malloc(sizeof(nmeaGPGSA))))
 *         goto mem_fail;
 *     node->packType = GPGSA;
 *     if(!nmea_parse_GPGSA(
 *         (const char *)parser->buffer + nparsed,
 *         sen_sz, (nmeaGPGSA *)node->pack))
 *     {
 *         free(node);
 *         node = 0;
 *     }
 *     break;
 * ...
 * \endcode
 */

#include "xnmea/tok.h"
#include "xnmea/parse.h"
#include "xnmea/context.h"
#include "xnmea/gmath.h"
#include "xnmea/units.h"

#include <string.h>
#include <stdio.h>

int _xnmea_parse_time(const char *buff, int buff_sz, xnmeaTIME *res)
{
    int success = 0;

    switch(buff_sz)
    {
    case sizeof("hhmmss") - 1:
        success = (3 == xnmea_scanf(buff, buff_sz,
            "%2d%2d%2d", &(res->hour), &(res->min), &(res->sec)
            ));
        break;
    case sizeof("hhmmss.s") - 1:
    case sizeof("hhmmss.ss") - 1:
    case sizeof("hhmmss.sss") - 1:
        success = (4 == xnmea_scanf(buff, buff_sz,
            "%2d%2d%2d.%d", &(res->hour), &(res->min), &(res->sec), &(res->hsec)
            ));
        break;
    default:
        xnmea_error("Parse of time error (format error)!");
        success = 0;
        break;
    }

    return (success?0:-1);
}

/**
 * \brief Define packet type by header (xnmeaPACKTYPE).
 * @param buff a constant character pointer of packet buffer.
 * @param buff_sz buffer size.
 * @return The defined packet type
 * @see xnmeaPACKTYPE
 */
int xnmea_pack_type(const char *buff, int buff_sz)
{
    static const char *pheads[] = {
        "GGA",
        "GSA",
        "GSV",
        "RMC",
        "VTG",
    };

    XNMEA_ASSERT(buff);

    if(buff_sz < 5)
        return NON;

    buff += 2;

    if(0 == memcmp(buff, pheads[0], 3))
        return GGA;
    else if(0 == memcmp(buff, pheads[1], 3))
        return GSA;
    else if(0 == memcmp(buff, pheads[2], 3))
        return GSV;
    else if(0 == memcmp(buff, pheads[3], 3))
        return RMC;
    else if(0 == memcmp(buff, pheads[4], 3))
        return VTG;

    return NON;
}

/**
 * \brief Find tail of packet ("\r\n") in buffer and check control sum (CRC).
 * @param buff a constant character pointer of packets buffer.
 * @param buff_sz buffer size.
 * @param res_crc a integer pointer for return CRC of packet (must be defined).
 * @return Number of bytes to packet tail.
 */
int xnmea_find_tail(const char *buff, int buff_sz, int *res_crc)
{
    static const int tail_sz = 3 /* *[CRC] */ + 2 /* \r\n */;

    const char *end_buff = buff + buff_sz;
    int nread = 0;
    int crc = 0;

    XNMEA_ASSERT(buff && res_crc);

    *res_crc = -1;

    for(;buff < end_buff; ++buff, ++nread)
    {
        if(('$' == *buff) && nread)
        {
            buff = 0;
            break;
        }
        else if('*' == *buff)
        {
            if(buff + tail_sz <= end_buff && '\r' == buff[3] && '\n' == buff[4])
            {
                *res_crc = xnmea_atoi(buff + 1, 2, 16);
                nread = buff_sz - (int)(end_buff - (buff + tail_sz));
                if(*res_crc != crc)
                {
                    *res_crc = -1;
                    buff = 0;
                }
            }

            break;
        }
        else if(nread)
            crc ^= (int)*buff;
    }

    if(*res_crc < 0 && buff)
        nread = 0;

    return nread;
}

/**
 * \brief Parse GGA packet from buffer.
 * @param buff a constant character pointer of packet buffer.
 * @param buff_sz buffer size.
 * @param pack a pointer of packet which will filled by function.
 * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
 */
int xnmea_parse_GGA(const char *buff, int buff_sz, xnmeaGGA *pack)
{
    char time_buff[XNMEA_TIMEPARSE_BUF];

    XNMEA_ASSERT(buff && pack);

    memset(pack, 0, sizeof(xnmeaGGA));

    xnmea_trace_buff(buff, buff_sz);

    buff += 3;
    buff_sz -= 3;

    if(14 != xnmea_scanf(buff, buff_sz,
        "GGA,%s,%f,%C,%f,%C,%d,%d,%f,%f,%C,%f,%C,%f,%d*",
        &(time_buff[0]),
        &(pack->lat), &(pack->ns), &(pack->lon), &(pack->ew),
        &(pack->sig), &(pack->satinuse), &(pack->HDOP), &(pack->elv), &(pack->elv_units),
        &(pack->diff), &(pack->diff_units), &(pack->dgps_age), &(pack->dgps_sid)))
    {
        xnmea_error("GGA parse error!");
        return 0;
    }

    if(0 != _xnmea_parse_time(&time_buff[0], (int)strlen(&time_buff[0]), &(pack->utc)))
    {
        xnmea_error("GGA time parse error!");
        return 0;
    }

    return 1;
}

/**
 * \brief Parse GSA packet from buffer.
 * @param buff a constant character pointer of packet buffer.
 * @param buff_sz buffer size.
 * @param pack a pointer of packet which will filled by function.
 * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
 */
int xnmea_parse_GSA(const char *buff, int buff_sz, xnmeaGSA *pack)
{
    XNMEA_ASSERT(buff && pack);

    memset(pack, 0, sizeof(xnmeaGSA));

    xnmea_trace_buff(buff, buff_sz);

    buff += 3;
    buff_sz -= 3;

    if(17 != xnmea_scanf(buff, buff_sz,
        "GSA,%C,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f*",
        &(pack->fix_mode), &(pack->fix_type),
        &(pack->sat_prn[0]), &(pack->sat_prn[1]), &(pack->sat_prn[2]), &(pack->sat_prn[3]), &(pack->sat_prn[4]), &(pack->sat_prn[5]),
        &(pack->sat_prn[6]), &(pack->sat_prn[7]), &(pack->sat_prn[8]), &(pack->sat_prn[9]), &(pack->sat_prn[10]), &(pack->sat_prn[11]),
        &(pack->PDOP), &(pack->HDOP), &(pack->VDOP)))
    {
        xnmea_error("GSA parse error!");
        return 0;
    }

    return 1;
}

/**
 * \brief Parse GSV packet from buffer.
 * @param buff a constant character pointer of packet buffer.
 * @param buff_sz buffer size.
 * @param pack a pointer of packet which will filled by function.
 * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
 */
int xnmea_parse_GSV(const char *buff, int buff_sz, xnmeaGSV *pack)
{
    int nsen, nsat;

    XNMEA_ASSERT(buff && pack);

    memset(pack, 0, sizeof(xnmeaGSV));

    xnmea_trace_buff(buff, buff_sz);

    buff += 3;
    buff_sz -= 3;

    nsen = xnmea_scanf(buff, buff_sz,
        "GSV,%d,%d,%d,"
        "%d,%d,%d,%d,"
        "%d,%d,%d,%d,"
        "%d,%d,%d,%d,"
        "%d,%d,%d,%d*",
        &(pack->pack_count), &(pack->pack_index), &(pack->sat_count),
        &(pack->sat_data[0].id), &(pack->sat_data[0].elv), &(pack->sat_data[0].azimuth), &(pack->sat_data[0].sig),
        &(pack->sat_data[1].id), &(pack->sat_data[1].elv), &(pack->sat_data[1].azimuth), &(pack->sat_data[1].sig),
        &(pack->sat_data[2].id), &(pack->sat_data[2].elv), &(pack->sat_data[2].azimuth), &(pack->sat_data[2].sig),
        &(pack->sat_data[3].id), &(pack->sat_data[3].elv), &(pack->sat_data[3].azimuth), &(pack->sat_data[3].sig));

    nsat = (pack->pack_index - 1) * XNMEA_SATINPACK;
    nsat = (nsat + XNMEA_SATINPACK > pack->sat_count)?pack->sat_count - nsat:XNMEA_SATINPACK;
    nsat = nsat * 4 + 3 /* first three sentence`s */;

    if(nsen < nsat || nsen > (XNMEA_SATINPACK * 4 + 3))
    {
        xnmea_error("GSV parse error!");
        return 0;
    }

    return 1;
}

/**
 * \brief Parse RMC packet from buffer.
 * @param buff a constant character pointer of packet buffer.
 * @param buff_sz buffer size.
 * @param pack a pointer of packet which will filled by function.
 * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
 */
int xnmea_parse_RMC(const char *buff, int buff_sz, xnmeaRMC *pack)
{
    int nsen;
    char time_buff[XNMEA_TIMEPARSE_BUF];

    XNMEA_ASSERT(buff && pack);

    memset(pack, 0, sizeof(xnmeaRMC));

    xnmea_trace_buff(buff, buff_sz);

    buff += 3;
    buff_sz -= 3;

    nsen = xnmea_scanf(buff, buff_sz,
        "RMC,%s,%C,%f,%C,%f,%C,%f,%f,%2d%2d%2d,%f,%C,%C*",
        &(time_buff[0]),
        &(pack->status), &(pack->lat), &(pack->ns), &(pack->lon), &(pack->ew),
        &(pack->speed), &(pack->direction),
        &(pack->utc.day), &(pack->utc.mon), &(pack->utc.year),
        &(pack->declination), &(pack->declin_ew), &(pack->mode));

    if(nsen != 13 && nsen != 14)
    {
        xnmea_error("RMC parse error!");
        return 0;
    }

    if(0 != _xnmea_parse_time(&time_buff[0], (int)strlen(&time_buff[0]), &(pack->utc)))
    {
        xnmea_error("RMC time parse error!");
        return 0;
    }

    if(pack->utc.year < 90)
        pack->utc.year += 100;
    pack->utc.mon -= 1;

    return 1;
}

/**
 * \brief Parse VTG packet from buffer.
 * @param buff a constant character pointer of packet buffer.
 * @param buff_sz buffer size.
 * @param pack a pointer of packet which will filled by function.
 * @return 1 (true) - if parsed successfully or 0 (false) - if fail.
 */
int xnmea_parse_VTG(const char *buff, int buff_sz, xnmeaVTG *pack)
{
    XNMEA_ASSERT(buff && pack);

    memset(pack, 0, sizeof(xnmeaVTG));

    xnmea_trace_buff(buff, buff_sz);

    buff += 3;
    buff_sz -= 3;

    if(8 != xnmea_scanf(buff, buff_sz,
        "VTG,%f,%C,%f,%C,%f,%C,%f,%C*",
        &(pack->dir), &(pack->dir_t),
        &(pack->dec), &(pack->dec_m),
        &(pack->spn), &(pack->spn_n),
        &(pack->spk), &(pack->spk_k)))
    {
        xnmea_error("VTG parse error!");
        return 0;
    }

    if( pack->dir_t != 'T' ||
        pack->dec_m != 'M' ||
        pack->spn_n != 'N' ||
        pack->spk_k != 'K')
    {
        xnmea_error("VTG parse error (format error)!");
        return 0;
    }

    return 1;
}

/**
 * \brief Fill xnmeaINFO structure by GGA packet data.
 * @param pack a pointer of packet structure.
 * @param info a pointer of summary information structure.
 */
void xnmea_GGA2info(xnmeaGGA *pack, xnmeaINFO *info)
{
    XNMEA_ASSERT(pack && info);

    info->utc.hour = pack->utc.hour;
    info->utc.min = pack->utc.min;
    info->utc.sec = pack->utc.sec;
    info->utc.hsec = pack->utc.hsec;
    info->sig = pack->sig;
    info->HDOP = pack->HDOP;
    info->elv = pack->elv;
    info->lat = ((pack->ns == 'N')?pack->lat:-(pack->lat));
    info->lon = ((pack->ew == 'E')?pack->lon:-(pack->lon));
    info->smask |= GGA;
}

/**
 * \brief Fill xnmeaINFO structure by GSA packet data.
 * @param pack a pointer of packet structure.
 * @param info a pointer of summary information structure.
 */
void xnmea_GSA2info(xnmeaGSA *pack, xnmeaINFO *info)
{
    int i, j, nuse = 0;

    XNMEA_ASSERT(pack && info);

    info->fix = pack->fix_type;
    info->PDOP = pack->PDOP;
    info->HDOP = pack->HDOP;
    info->VDOP = pack->VDOP;

    for(i = 0; i < XNMEA_MAXSAT; ++i)
    {
        for(j = 0; j < info->satinfo.inview; ++j)
        {
            if(pack->sat_prn[i] && pack->sat_prn[i] == info->satinfo.sat[j].id)
            {
                info->satinfo.sat[j].in_use = 1;
                nuse++;
            }
        }
    }

    info->satinfo.inuse = nuse;
    info->smask |= GSA;
}

/**
 * \brief Fill xnmeaINFO structure by GSV packet data.
 * @param pack a pointer of packet structure.
 * @param info a pointer of summary information structure.
 */
void xnmea_GSV2info(xnmeaGSV *pack, xnmeaINFO *info)
{
    int isat, isi, nsat;

    XNMEA_ASSERT(pack && info);

    if(pack->pack_index > pack->pack_count ||
        pack->pack_index * XNMEA_SATINPACK > XNMEA_MAXSAT)
        return;

    if(pack->pack_index < 1)
        pack->pack_index = 1;

    info->satinfo.inview = pack->sat_count;

    nsat = (pack->pack_index - 1) * XNMEA_SATINPACK;
    nsat = (nsat + XNMEA_SATINPACK > pack->sat_count)?pack->sat_count - nsat:XNMEA_SATINPACK;

    for(isat = 0; isat < nsat; ++isat)
    {
        isi = (pack->pack_index - 1) * XNMEA_SATINPACK + isat;
        info->satinfo.sat[isi].id = pack->sat_data[isat].id;
        info->satinfo.sat[isi].elv = pack->sat_data[isat].elv;
        info->satinfo.sat[isi].azimuth = pack->sat_data[isat].azimuth;
        info->satinfo.sat[isi].sig = pack->sat_data[isat].sig;
    }

    info->smask |= GSV;
}

/**
 * \brief Fill xnmeaINFO structure by RMC packet data.
 * @param pack a pointer of packet structure.
 * @param info a pointer of summary information structure.
 */
void xnmea_RMC2info(xnmeaRMC *pack, xnmeaINFO *info)
{
    XNMEA_ASSERT(pack && info);

    if('A' == pack->status)
    {
        if(XNMEA_SIG_BAD == info->sig)
            info->sig = XNMEA_SIG_MID;
        if(XNMEA_FIX_BAD == info->fix)
            info->fix = XNMEA_FIX_2D;
    }
    else if('V' == pack->status)
    {
        info->sig = XNMEA_SIG_BAD;
        info->fix = XNMEA_FIX_BAD;
    }

    info->utc = pack->utc;
    info->lat = ((pack->ns == 'N')?pack->lat:-(pack->lat));
    info->lon = ((pack->ew == 'E')?pack->lon:-(pack->lon));
    info->speed = pack->speed * XNMEA_TUD_KNOTS;
    info->direction = pack->direction;
    info->smask |= RMC;
}

/**
 * \brief Fill xnmeaINFO structure by VTG packet data.
 * @param pack a pointer of packet structure.
 * @param info a pointer of summary information structure.
 */
void xnmea_VTG2info(xnmeaVTG *pack, xnmeaINFO *info)
{
    XNMEA_ASSERT(pack && info);

    info->direction = pack->dir;
    info->declination = pack->dec;
    info->speed = pack->spk;
    info->smask |= VTG;
}
