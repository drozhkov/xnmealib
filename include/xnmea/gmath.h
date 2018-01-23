/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: gmath.h 17 2008-03-11 11:56:11Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

#ifndef __XNMEA_GMATH_H__
#define __XNMEA_GMATH_H__

#include "info.h"

#define XNMEA_PI                     (3.141592653589793)                /**< PI value */
#define XNMEA_PI180                  (XNMEA_PI / 180)                   /**< PI division by 180 */
#define XNMEA_EARTHRADIUS_KM         (6378)                             /**< Earth's mean radius in km */
#define XNMEA_EARTHRADIUS_M          (XNMEA_EARTHRADIUS_KM * 1000)      /**< Earth's mean radius in m */
#define XNMEA_EARTH_SEMIMAJORAXIS_M  (6378137.0)                        /**< Earth's semi-major axis in m according WGS84 */
#define XNMEA_EARTH_SEMIMAJORAXIS_KM (XNMEA_EARTHMAJORAXIS_KM / 1000)   /**< Earth's semi-major axis in km according WGS 84 */
#define XNMEA_EARTH_FLATTENING       (1 / 298.257223563)                /**< Earth's flattening according WGS 84 */
#define XNMEA_DOP_FACTOR             (5)                                /**< Factor for translating DOP to meters */

#ifdef  __cplusplus
extern "C" {
#endif

/*
 * degree VS radian
 */

double xnmea_degree2radian(double val);
double xnmea_radian2degree(double val);

/*
 * NDEG (NMEA degree)
 */

double xnmea_ndeg2degree(double val);
double xnmea_degree2ndeg(double val);

double xnmea_ndeg2radian(double val);
double xnmea_radian2ndeg(double val);

/*
 * DOP
 */

double xnmea_calc_pdop(double hdop, double vdop);
double xnmea_dop2meters(double dop);
double xnmea_meters2dop(double meters);

/*
 * positions work
 */

void xnmea_info2pos(const xnmeaINFO *info, xnmeaPOS *pos);
void xnmea_pos2info(const xnmeaPOS *pos, xnmeaINFO *info);

double  xnmea_distance(
        const xnmeaPOS *from_pos,
        const xnmeaPOS *to_pos
        );

double  xnmea_distance_ellipsoid(
        const xnmeaPOS *from_pos,
        const xnmeaPOS *to_pos,
        double *from_azimuth,
        double *to_azimuth
        );

int     xnmea_move_horz(
        const xnmeaPOS *start_pos,
        xnmeaPOS *end_pos,
        double azimuth,
        double distance
        );

int     xnmea_move_horz_ellipsoid(
        const xnmeaPOS *start_pos,
        xnmeaPOS *end_pos,
        double azimuth,
        double distance,
        double *end_azimuth
        );

#ifdef  __cplusplus
}
#endif

#endif /* __XNMEA_GMATH_H__ */
