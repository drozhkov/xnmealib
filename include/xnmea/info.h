/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: info.h 10 2007-11-15 14:50:15Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

/*! \file */

#ifndef __XNMEA_INFO_H__
#define __XNMEA_INFO_H__

#include "time.h"

#define XNMEA_SIG_BAD        (0)
#define XNMEA_SIG_LOW        (1)
#define XNMEA_SIG_MID        (2)
#define XNMEA_SIG_HIGH       (3)

#define XNMEA_FIX_BAD        (1)
#define XNMEA_FIX_2D         (2)
#define XNMEA_FIX_3D         (3)

#define XNMEA_MAXSAT         (12)
#define XNMEA_SATINPACK      (4)
#define XNMEA_NSATPACKS      (XNMEA_MAXSAT / XNMEA_SATINPACK)

#define XNMEA_DEF_LAT        (5001.2621)
#define XNMEA_DEF_LON        (3613.0595)

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * Position data in fractional degrees or radians
 */
typedef struct _xnmeaPOS
{
    double lat;         /**< Latitude */
    double lon;         /**< Longitude */

} xnmeaPOS;

/**
 * Information about satellite
 * @see xnmeaSATINFO
 * @see xnmeaGSV
 */
typedef struct _xnmeaSATELLITE
{
    int     id;         /**< Satellite PRN number */
    int     in_use;     /**< Used in position fix */
    int     elv;        /**< Elevation in degrees, 90 maximum */
    int     azimuth;    /**< Azimuth, degrees from true north, 000 to 359 */
    int     sig;        /**< Signal, 00-99 dB */

} xnmeaSATELLITE;

/**
 * Information about all satellites in view
 * @see xnmeaINFO
 * @see xnmeaGSV
 */
typedef struct _xnmeaSATINFO
{
    int     inuse;      /**< Number of satellites in use (not those in view) */
    int     inview;     /**< Total number of satellites in view */
    xnmeaSATELLITE sat[XNMEA_MAXSAT]; /**< Satellites information */

} xnmeaSATINFO;

/**
 * Summary GPS information from all parsed packets,
 * used also for generating NMEA stream
 * @see xnmea_parse
 * @see xnmea_GGA2info,  xnmea_...2info
 */
typedef struct _xnmeaINFO
{
    int     smask;      /**< Mask specifying types of packages from which data have been obtained */

    xnmeaTIME utc;       /**< UTC of position */

    int     sig;        /**< GPS quality indicator (0 = Invalid; 1 = Fix; 2 = Differential, 3 = Sensitive) */
    int     fix;        /**< Operating mode, used for navigation (1 = Fix not available; 2 = 2D; 3 = 3D) */

    double  PDOP;       /**< Position Dilution Of Precision */
    double  HDOP;       /**< Horizontal Dilution Of Precision */
    double  VDOP;       /**< Vertical Dilution Of Precision */

    double  lat;        /**< Latitude in NDEG - +/-[degree][min].[sec/60] */
    double  lon;        /**< Longitude in NDEG - +/-[degree][min].[sec/60] */
    double  elv;        /**< Antenna altitude above/below mean sea level (geoid) in meters */
    double  speed;      /**< Speed over the ground in kilometers/hour */
    double  direction;  /**< Track angle in degrees True */
    double  declination; /**< Magnetic variation degrees (Easterly var. subtracts from true course) */

    xnmeaSATINFO satinfo; /**< Satellites information */

} xnmeaINFO;

void xnmea_zero_INFO(xnmeaINFO *info);

#ifdef  __cplusplus
}
#endif

#endif /* __XNMEA_INFO_H__ */
