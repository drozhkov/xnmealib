/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: generator.c 17 2008-03-11 11:56:11Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

#include "xnmea/gmath.h"
#include "xnmea/generate.h"
#include "xnmea/generator.h"
#include "xnmea/context.h"

#include <string.h>
#include <stdlib.h>

#if defined(XNMEA_WIN) && defined(_MSC_VER)
# pragma warning(disable: 4100) /* unreferenced formal parameter */
#endif

double xnmea_random(double min, double max)
{
    static double rand_max = RAND_MAX;
    double rand_val = rand();
    double bounds = max - min;
    return min + (rand_val * bounds) / rand_max;
}

/*
 * low level
 */

int xnmea_gen_init(xnmeaGENERATOR *gen, xnmeaINFO *info)
{
    int RetVal = 1; int smask = info->smask;
    xnmeaGENERATOR *igen = gen;

    xnmea_zero_INFO(info);
    info->smask = smask;

    info->lat = XNMEA_DEF_LAT;
    info->lon = XNMEA_DEF_LON;

    while(RetVal && igen)
    {
        if(igen->init_call)
            RetVal = (*igen->init_call)(igen, info);
        igen = igen->next;
    }

    return RetVal;
}

int xnmea_gen_loop(xnmeaGENERATOR *gen, xnmeaINFO *info)
{
    int RetVal = 1;

    if(gen->loop_call)
        RetVal = (*gen->loop_call)(gen, info);

    if(RetVal && gen->next)
        RetVal = xnmea_gen_loop(gen->next, info);

    return RetVal;
}

int xnmea_gen_reset(xnmeaGENERATOR *gen, xnmeaINFO *info)
{
    int RetVal = 1;

    if(gen->reset_call)
        RetVal = (*gen->reset_call)(gen, info);

    return RetVal;
}

void xnmea_gen_destroy(xnmeaGENERATOR *gen)
{
    if(gen->next)
    {
        xnmea_gen_destroy(gen->next);
        gen->next = 0;
    }

    if(gen->destroy_call)
        (*gen->destroy_call)(gen);

    free(gen);
}

void xnmea_gen_add(xnmeaGENERATOR *to, xnmeaGENERATOR *gen)
{
    if(to->next)
        xnmea_gen_add(to->next, gen);
    else
        to->next = gen;
}

int xnmea_generate_from(
    char *buff, int buff_sz,
    xnmeaINFO *info,
    xnmeaGENERATOR *gen,
    int generate_mask,
    xnmeaSOURCE source
    )
{
    int retval;

    if(0 != (retval = xnmea_gen_loop(gen, info)))
        retval = xnmea_generate(buff, buff_sz, info, generate_mask, source);

    return retval;
}

/*
 * NOISE generator
 */

int xnmea_igen_noise_init(xnmeaGENERATOR *gen, xnmeaINFO *info)
{
    return 1;
}

int xnmea_igen_noise_loop(xnmeaGENERATOR *gen, xnmeaINFO *info)
{
    int it;
    int in_use;

    info->sig = (int)xnmea_random(1, 3);
    info->PDOP = xnmea_random(0, 9);
    info->HDOP = xnmea_random(0, 9);
    info->VDOP = xnmea_random(0, 9);
    info->fix = (int)xnmea_random(2, 3);
    info->lat = xnmea_random(0, 100);
    info->lon = xnmea_random(0, 100);
    info->speed = xnmea_random(0, 100);
    info->direction = xnmea_random(0, 360);
    info->declination = xnmea_random(0, 360);
    info->elv = (int)xnmea_random(-100, 100);

    info->satinfo.inuse = 0;
    info->satinfo.inview = 0;

    for(it = 0; it < 12; ++it)
    {
        info->satinfo.sat[it].id = it;
        info->satinfo.sat[it].in_use = in_use = (int)xnmea_random(0, 3);
        info->satinfo.sat[it].elv = (int)xnmea_random(0, 90);
        info->satinfo.sat[it].azimuth = (int)xnmea_random(0, 359);
        info->satinfo.sat[it].sig = (int)(in_use?xnmea_random(40, 99):xnmea_random(0, 40));

        if(in_use)
            info->satinfo.inuse++;
        if(info->satinfo.sat[it].sig > 0)
            info->satinfo.inview++;
    }

    return 1;
}

int xnmea_igen_noise_reset(xnmeaGENERATOR *gen, xnmeaINFO *info)
{
    return 1;
}

/*
 * STATIC generator
 */

int xnmea_igen_static_loop(xnmeaGENERATOR *gen, xnmeaINFO *info)
{
    xnmea_time_now(&info->utc);
    return 1;
};

int xnmea_igen_static_reset(xnmeaGENERATOR *gen, xnmeaINFO *info)
{
    info->satinfo.inuse = 4;
    info->satinfo.inview = 4;

    info->satinfo.sat[0].id = 1;
    info->satinfo.sat[0].in_use = 1;
    info->satinfo.sat[0].elv = 50;
    info->satinfo.sat[0].azimuth = 0;
    info->satinfo.sat[0].sig = 99;

    info->satinfo.sat[1].id = 2;
    info->satinfo.sat[1].in_use = 1;
    info->satinfo.sat[1].elv = 50;
    info->satinfo.sat[1].azimuth = 90;
    info->satinfo.sat[1].sig = 99;

    info->satinfo.sat[2].id = 3;
    info->satinfo.sat[2].in_use = 1;
    info->satinfo.sat[2].elv = 50;
    info->satinfo.sat[2].azimuth = 180;
    info->satinfo.sat[2].sig = 99;

    info->satinfo.sat[3].id = 4;
    info->satinfo.sat[3].in_use = 1;
    info->satinfo.sat[3].elv = 50;
    info->satinfo.sat[3].azimuth = 270;
    info->satinfo.sat[3].sig = 99;

    return 1;
}

int xnmea_igen_static_init(xnmeaGENERATOR *gen, xnmeaINFO *info)
{
    info->sig = 3;
    info->fix = 3;

    xnmea_igen_static_reset(gen, info);

    return 1;
}

/*
 * SAT_ROTATE generator
 */

int xnmea_igen_rotate_loop(xnmeaGENERATOR *gen, xnmeaINFO *info)
{
    int it;
    int count = info->satinfo.inview;
    double deg = 360 / (count?count:1);
    double srt = (count?(info->satinfo.sat[0].azimuth):0) + 5;

    xnmea_time_now(&info->utc);

    for(it = 0; it < count; ++it)
    {
        info->satinfo.sat[it].azimuth =
            (int)((srt >= 360)?srt - 360:srt);
        srt += deg;
    }

    return 1;
};

int xnmea_igen_rotate_reset(xnmeaGENERATOR *gen, xnmeaINFO *info)
{
    int it;
    double deg = 360 / 8;
    double srt = 0;

    info->satinfo.inuse = 8;
    info->satinfo.inview = 8;

    for(it = 0; it < info->satinfo.inview; ++it)
    {
        info->satinfo.sat[it].id = it + 1;
        info->satinfo.sat[it].in_use = 1;
        info->satinfo.sat[it].elv = 5;
        info->satinfo.sat[it].azimuth = (int)srt;
        info->satinfo.sat[it].sig = 80;
        srt += deg;
    }

    return 1;
}

int xnmea_igen_rotate_init(xnmeaGENERATOR *gen, xnmeaINFO *info)
{
    info->sig = 3;
    info->fix = 3;

    xnmea_igen_rotate_reset(gen, info);

    return 1;
}

/*
 * POS_RANDMOVE generator
 */

int xnmea_igen_pos_rmove_init(xnmeaGENERATOR *gen, xnmeaINFO *info)
{
    info->sig = 3;
    info->fix = 3;
    info->direction = info->declination = 0;
    info->speed = 20;
    return 1;
}

int xnmea_igen_pos_rmove_loop(xnmeaGENERATOR *gen, xnmeaINFO *info)
{
    xnmeaPOS crd;

    info->direction += xnmea_random(-10, 10);
    info->speed += xnmea_random(-2, 3);

    if(info->direction < 0)
        info->direction = 359 + info->direction;
    if(info->direction > 359)
        info->direction -= 359;

    if(info->speed > 40)
        info->speed = 40;
    if(info->speed < 1)
        info->speed = 1;

    xnmea_info2pos(info, &crd);
    xnmea_move_horz(&crd, &crd, info->direction, info->speed / 3600);
    xnmea_pos2info(&crd, info);

    info->declination = info->direction;

    return 1;
};

int xnmea_igen_pos_rmove_destroy(xnmeaGENERATOR *gen)
{
    return 1;
};

/*
 * generator create
 */

xnmeaGENERATOR * __xnmea_create_generator(int type, xnmeaINFO *info)
{
    xnmeaGENERATOR *gen = 0;

    switch(type)
    {
    case XNMEA_GEN_NOISE:
        if(0 == (gen = malloc(sizeof(xnmeaGENERATOR))))
            xnmea_error("Insufficient memory!");
        else
        {
            memset(gen, 0, sizeof(xnmeaGENERATOR));
            gen->init_call = &xnmea_igen_noise_init;
            gen->loop_call = &xnmea_igen_noise_loop;
            gen->reset_call = &xnmea_igen_noise_reset;
        }
        break;
    case XNMEA_GEN_STATIC:
    case XNMEA_GEN_SAT_STATIC:
        if(0 == (gen = malloc(sizeof(xnmeaGENERATOR))))
            xnmea_error("Insufficient memory!");
        else
        {
            memset(gen, 0, sizeof(xnmeaGENERATOR));
            gen->init_call = &xnmea_igen_static_init;
            gen->loop_call = &xnmea_igen_static_loop;
            gen->reset_call = &xnmea_igen_static_reset;
        }
        break;
    case XNMEA_GEN_SAT_ROTATE:
        if(0 == (gen = malloc(sizeof(xnmeaGENERATOR))))
            xnmea_error("Insufficient memory!");
        else
        {
            memset(gen, 0, sizeof(xnmeaGENERATOR));
            gen->init_call = &xnmea_igen_rotate_init;
            gen->loop_call = &xnmea_igen_rotate_loop;
            gen->reset_call = &xnmea_igen_rotate_reset;
        }
        break;
    case XNMEA_GEN_POS_RANDMOVE:
        if(0 == (gen = malloc(sizeof(xnmeaGENERATOR))))
            xnmea_error("Insufficient memory!");
        else
        {
            memset(gen, 0, sizeof(xnmeaGENERATOR));
            gen->init_call = &xnmea_igen_pos_rmove_init;
            gen->loop_call = &xnmea_igen_pos_rmove_loop;
            gen->destroy_call = &xnmea_igen_pos_rmove_destroy;
        }
        break;
    case XNMEA_GEN_ROTATE:
        gen = __xnmea_create_generator(XNMEA_GEN_SAT_ROTATE, info);
        xnmea_gen_add(gen, __xnmea_create_generator(XNMEA_GEN_POS_RANDMOVE, info));
        break;
    };

    return gen;
}

xnmeaGENERATOR * xnmea_create_generator(int type, xnmeaINFO *info)
{
    xnmeaGENERATOR *gen = __xnmea_create_generator(type, info);

    if(gen)
        xnmea_gen_init(gen, info);

    return gen;
}

void xnmea_destroy_generator(xnmeaGENERATOR *gen)
{
    xnmea_gen_destroy(gen);
}

#if defined(XNMEA_WIN) && defined(_MSC_VER)
# pragma warning(default: 4100)
#endif
