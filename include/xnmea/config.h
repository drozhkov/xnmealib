/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: config.h 17 2008-03-11 11:56:11Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

#ifndef __XNMEA_CONFIG_H__
#define __XNMEA_CONFIG_H__

#define XNMEA_VERSION        ("0.0.1")
#define XNMEA_VERSION_MAJOR  (0)
#define XNMEA_VERSION_MINOR  (0)
#define XNMEA_VERSION_PATCH  (1)

#define XNMEA_CONVSTR_BUF    (256)
#define XNMEA_TIMEPARSE_BUF  (256)

#if defined(WINCE) || defined(UNDER_CE)
#   define  XNMEA_CE
#endif

#if defined(WIN32) || defined(XNMEA_CE)
#   define  XNMEA_WIN
#else
#   define  XNMEA_UNI
#endif

#if defined(XNMEA_WIN) && (_MSC_VER >= 1400)
# pragma warning(disable: 4996) /* declared deprecated */
#endif

#if defined(_MSC_VER)
# define XNMEA_POSIX(x)  _##x
# define XNMEA_INLINE    __inline
#else
# define XNMEA_POSIX(x)  x
# define XNMEA_INLINE    inline
#endif

#if !defined(NDEBUG) && !defined(NMEA_CE)
#   include <assert.h>
#   define XNMEA_ASSERT(x)   assert(x)
#else
#   define XNMEA_ASSERT(x)
#endif

#endif /* __XNMEA_CONFIG_H__ */
