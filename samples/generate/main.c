#include <xnmea/xnmea.h>
#include <stdio.h>

#ifdef XNMEA_WIN
#   include <windows.h>
#else
#   include <unistd.h>
#endif

int main()
{
    xnmeaINFO info;
    char buff[2048];
    int gen_sz;
    int it;

    xnmea_zero_INFO(&info);

    info.sig = 3;
    info.fix = 3;
    info.lat = 5000.0;
    info.lon = 3600.0;
    info.speed = 2.14 * XNMEA_TUS_MS;
    info.elv = 10.86;

    info.satinfo.inuse = 1;
    info.satinfo.inview = 1;

    /*
    info.satinfo.sat[0].id = 1;
    info.satinfo.sat[0].in_use = 1;
    info.satinfo.sat[0].elv = 50;
    info.satinfo.sat[0].azimuth = 0;
    info.satinfo.sat[0].sig = 99;
    */

    for(it = 0; it < 10; ++it)
    {
        gen_sz = xnmea_generate(
            &buff[0], 2048, &info,
            GGA | GSA | GSV | RMC | VTG,
            XNMEA_SOURCE_GPS );

        buff[gen_sz] = 0;
        printf("%s\n", &buff[0]);

#ifdef XNMEA_WIN
        Sleep(500);
#else
        usleep(500000);
#endif

        info.speed += .1;
    }

    return 0;
}
