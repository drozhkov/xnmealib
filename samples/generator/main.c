#include <xnmea/xnmea.h>
#include <stdio.h>

#ifdef XNMEA_WIN
#   include <windows.h>
#else
#   include <unistd.h>
#endif

int main()
{
    xnmeaGENERATOR *gen;
    xnmeaINFO info;
    char buff[2048];
    int gen_sz;
    int it;

    xnmea_zero_INFO(&info);

    if(0 == (gen = xnmea_create_generator(XNMEA_GEN_ROTATE, &info)))
        return -1;

    for(it = 0; it < 10000; ++it)
    {
        gen_sz = xnmea_generate_from(
            &buff[0], 2048, &info, gen,
            GGA | GSA | GSV | RMC | VTG,
            XNMEA_SOURCE_GPS
            );

        buff[gen_sz] = 0;
        printf("%s\n", &buff[0]);

#ifdef XNMEA_WIN
        Sleep(500);
#else
        usleep(500000);
#endif
    }

    xnmea_gen_destroy(gen);

    return 0;
}
