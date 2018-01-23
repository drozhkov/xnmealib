#include <xnmea/xnmea.h>

#include <string.h>
#include <stdio.h>

#ifdef XNMEA_WIN
#   include <io.h>
#endif

void trace(const char *str, int str_size)
{
    printf("Trace: ");
    write(1, str, str_size);
    printf("\n");
}
void error(const char *str, int str_size)
{
    printf("Error: ");
    write(1, str, str_size);
    printf("\n");
}

int main()
{
    xnmeaINFO info;
    xnmeaPARSER parser;
    FILE *file;
    char buff[2048];
    int size, it = 0;
    xnmeaPOS dpos;

    file = fopen("gpslog.txt", "rb");

    if(!file)
        return -1;

    xnmea_property()->trace_func = &trace;
    xnmea_property()->error_func = &error;

    xnmea_zero_INFO(&info);
    xnmea_parser_init(&parser);

    /*
    while(1)
    {
    */

    while(!feof(file))
    {
        size = (int)fread(&buff[0], 1, 100, file);

        xnmea_parse(&parser, &buff[0], size, &info);

        xnmea_info2pos(&info, &dpos);

        printf(
            "%03d, Lat: %f, Lon: %f, Sig: %d, Fix: %d\n",
            it++, dpos.lat, dpos.lon, info.sig, info.fix
            );
    }

    fseek(file, 0, SEEK_SET);

    /*
    }
    */

    xnmea_parser_destroy(&parser);
    fclose(file);

    return 0;
}
