/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: generator.h 4 2007-08-27 13:11:03Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

#ifndef __XNMEA_GENERATOR_H__
#define __XNMEA_GENERATOR_H__

#include "info.h"

#ifdef  __cplusplus
extern "C" {
#endif

/*
 * high level
 */

struct _xnmeaGENERATOR;

enum xnmeaGENTYPE
{
    XNMEA_GEN_NOISE = 0,
    XNMEA_GEN_STATIC,
    XNMEA_GEN_ROTATE,

    XNMEA_GEN_SAT_STATIC,
    XNMEA_GEN_SAT_ROTATE,
    XNMEA_GEN_POS_RANDMOVE,

    XNMEA_GEN_LAST
};

struct _xnmeaGENERATOR * xnmea_create_generator(int type, xnmeaINFO *info);
void    xnmea_destroy_generator(struct _xnmeaGENERATOR *gen);

int     xnmea_generate_from(
        char *buff, int buff_sz,        /* buffer */
        xnmeaINFO *info,                /* source info */
        struct _xnmeaGENERATOR *gen,    /* generator */
        int generate_mask,              /* mask of sentence`s (e.g. XNMEA_PACKTYPE_GGA | XNMEA_PACKTYPE_GSA) */
        xnmeaSOURCE source
        );

/*
 * low level
 */

typedef int (*xnmeaNMEA_GEN_INIT)(struct _xnmeaGENERATOR *gen, xnmeaINFO *info);
typedef int (*xnmeaNMEA_GEN_LOOP)(struct _xnmeaGENERATOR *gen, xnmeaINFO *info);
typedef int (*xnmeaNMEA_GEN_RESET)(struct _xnmeaGENERATOR *gen, xnmeaINFO *info);
typedef int (*xnmeaNMEA_GEN_DESTROY)(struct _xnmeaGENERATOR *gen);

typedef struct _xnmeaGENERATOR
{
    void                *gen_data;
    xnmeaNMEA_GEN_INIT    init_call;
    xnmeaNMEA_GEN_LOOP    loop_call;
    xnmeaNMEA_GEN_RESET   reset_call;
    xnmeaNMEA_GEN_DESTROY destroy_call;
    struct _xnmeaGENERATOR *next;

} xnmeaGENERATOR;

int     xnmea_gen_init(xnmeaGENERATOR *gen, xnmeaINFO *info);
int     xnmea_gen_loop(xnmeaGENERATOR *gen, xnmeaINFO *info);
int     xnmea_gen_reset(xnmeaGENERATOR *gen, xnmeaINFO *info);
void    xnmea_gen_destroy(xnmeaGENERATOR *gen);
void    xnmea_gen_add(xnmeaGENERATOR *to, xnmeaGENERATOR *gen);

#ifdef  __cplusplus
}
#endif

#endif /* __XNMEA_GENERATOR_H__ */
