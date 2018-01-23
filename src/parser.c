/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: parser.c 17 2008-03-11 11:56:11Z xtimor $
 *
 */

/*
 *
 * xNMEA library
 * URL: https://github.com/drozhkov/xnmealib
 *
 */

/**
 * \file parser.h
 */

#include "xnmea/tok.h"
#include "xnmea/parse.h"
#include "xnmea/parser.h"
#include "xnmea/context.h"

#include <string.h>
#include <stdlib.h>

typedef struct _xnmeaParserNODE
{
    int packType;
    void *pack;
    struct _xnmeaParserNODE *next_node;

} xnmeaParserNODE;

/*
 * high level
 */

/**
 * \brief Initialization of parser object
 * @return true (1) - success or false (0) - fail
 */
int xnmea_parser_init(xnmeaPARSER *parser)
{
    int resv = 0;
    int buff_size = xnmea_property()->parse_buff_size;

    XNMEA_ASSERT(parser);

    if(buff_size < XNMEA_MIN_PARSEBUFF)
        buff_size = XNMEA_MIN_PARSEBUFF;

    memset(parser, 0, sizeof(xnmeaPARSER));

    if(0 == (parser->buffer = malloc(buff_size)))
        xnmea_error("Insufficient memory!");
    else
    {
        parser->buff_size = buff_size;
        resv = 1;
    }

    return resv;
}

/**
 * \brief Destroy parser object
 */
void xnmea_parser_destroy(xnmeaPARSER *parser)
{
    XNMEA_ASSERT(parser && parser->buffer);
    free(parser->buffer);
    xnmea_parser_queue_clear(parser);
    memset(parser, 0, sizeof(xnmeaPARSER));
}

/**
 * \brief Analysis of buffer and put results to information structure
 * @return Number of packets wos parsed
 */
int xnmea_parse(
    xnmeaPARSER *parser,
    const char *buff, int buff_sz,
    xnmeaINFO *info
    )
{
    int ptype, nread = 0;
    void *pack = 0;

    XNMEA_ASSERT(parser && parser->buffer);

    xnmea_parser_push(parser, buff, buff_sz);

    while(NON != (ptype = xnmea_parser_pop(parser, &pack)))
    {
        nread++;

        switch(ptype)
        {
        case GGA:
            xnmea_GGA2info((xnmeaGGA *)pack, info);
            break;
        case GSA:
            xnmea_GSA2info((xnmeaGSA *)pack, info);
            break;
        case GSV:
            xnmea_GSV2info((xnmeaGSV *)pack, info);
            break;
        case RMC:
            xnmea_RMC2info((xnmeaRMC *)pack, info);
            break;
        case VTG:
            xnmea_VTG2info((xnmeaVTG *)pack, info);
            break;
        };

        free(pack);
    }

    return nread;
}

/*
 * low level
 */

int xnmea_parser_real_push(xnmeaPARSER *parser, const char *buff, int buff_sz)
{
    int nparsed = 0, crc, sen_sz, ptype;
    xnmeaParserNODE *node = 0;

    XNMEA_ASSERT(parser && parser->buffer);

    /* clear unuse buffer (for debug) */
    /*
    memset(
        parser->buffer + parser->buff_use, 0,
        parser->buff_size - parser->buff_use
        );
        */

    /* add */
    if(parser->buff_use + buff_sz >= parser->buff_size)
        xnmea_parser_buff_clear(parser);

    memcpy(parser->buffer + parser->buff_use, buff, buff_sz);
    parser->buff_use += buff_sz;

    /* parse */
    for(;;node = 0)
    {
        sen_sz = xnmea_find_tail(
            (const char *)parser->buffer + nparsed,
            (int)parser->buff_use - nparsed, &crc);

        if(!sen_sz)
        {
            if(nparsed)
                memcpy(
                parser->buffer,
                parser->buffer + nparsed,
                parser->buff_use -= nparsed);
            break;
        }
        else if(crc >= 0)
        {
            ptype = xnmea_pack_type(
                (const char *)parser->buffer + nparsed + 1,
                parser->buff_use - nparsed - 1);

            if(0 == (node = malloc(sizeof(xnmeaParserNODE))))
                goto mem_fail;

            node->pack = 0;

            switch(ptype)
            {
            case GGA:
                if(0 == (node->pack = malloc(sizeof(xnmeaGGA))))
                    goto mem_fail;
                node->packType = GGA;
                if(!xnmea_parse_GGA(
                    (const char *)parser->buffer + nparsed,
                    sen_sz, (xnmeaGGA *)node->pack))
                {
                    free(node);
                    node = 0;
                }
                break;
            case GSA:
                if(0 == (node->pack = malloc(sizeof(xnmeaGSA))))
                    goto mem_fail;
                node->packType = GSA;
                if(!xnmea_parse_GSA(
                    (const char *)parser->buffer + nparsed,
                    sen_sz, (xnmeaGSA *)node->pack))
                {
                    free(node);
                    node = 0;
                }
                break;
            case GSV:
                if(0 == (node->pack = malloc(sizeof(xnmeaGSV))))
                    goto mem_fail;
                node->packType = GSV;
                if(!xnmea_parse_GSV(
                    (const char *)parser->buffer + nparsed,
                    sen_sz, (xnmeaGSV *)node->pack))
                {
                    free(node);
                    node = 0;
                }
                break;
            case RMC:
                if(0 == (node->pack = malloc(sizeof(xnmeaRMC))))
                    goto mem_fail;
                node->packType = RMC;
                if(!xnmea_parse_RMC(
                    (const char *)parser->buffer + nparsed,
                    sen_sz, (xnmeaRMC *)node->pack))
                {
                    free(node);
                    node = 0;
                }
                break;
            case VTG:
                if(0 == (node->pack = malloc(sizeof(xnmeaVTG))))
                    goto mem_fail;
                node->packType = VTG;
                if(!xnmea_parse_VTG(
                    (const char *)parser->buffer + nparsed,
                    sen_sz, (xnmeaVTG *)node->pack))
                {
                    free(node);
                    node = 0;
                }
                break;
            default:
                free(node);
                node = 0;
                break;
            };

            if(node)
            {
                if(parser->end_node)
                    ((xnmeaParserNODE *)parser->end_node)->next_node = node;
                parser->end_node = node;
                if(!parser->top_node)
                    parser->top_node = node;
                node->next_node = 0;
            }
        }

        nparsed += sen_sz;
    }

    return nparsed;

mem_fail:
    if(node)
        free(node);

    xnmea_error("Insufficient memory!");

    return -1;
}

/**
 * \brief Analysis of buffer and keep results into parser
 * @return Number of bytes wos parsed from buffer
 */
int xnmea_parser_push(xnmeaPARSER *parser, const char *buff, int buff_sz)
{
    int nparse, nparsed = 0;

    do
    {
        if(buff_sz > parser->buff_size)
            nparse = parser->buff_size;
        else
            nparse = buff_sz;

        nparsed += xnmea_parser_real_push(
            parser, buff, nparse);

        buff_sz -= nparse;

    } while(buff_sz);

    return nparsed;
}

/**
 * \brief Get type of top packet keeped into parser
 * @return Type of packet
 * @see xnmeaPACKTYPE
 */
int xnmea_parser_top(xnmeaPARSER *parser)
{
    int retval = NON;
    xnmeaParserNODE *node = (xnmeaParserNODE *)parser->top_node;

    XNMEA_ASSERT(parser && parser->buffer);

    if(node)
        retval = node->packType;

    return retval;
}

/**
 * \brief Withdraw top packet from parser
 * @return Received packet type
 * @see xnmeaPACKTYPE
 */
int xnmea_parser_pop(xnmeaPARSER *parser, void **pack_ptr)
{
    int retval = NON;
    xnmeaParserNODE *node = (xnmeaParserNODE *)parser->top_node;

    XNMEA_ASSERT(parser && parser->buffer);

    if(node)
    {
        *pack_ptr = node->pack;
        retval = node->packType;
        parser->top_node = node->next_node;
        if(!parser->top_node)
            parser->end_node = 0;
        free(node);
    }

    return retval;
}

/**
 * \brief Get top packet from parser without withdraw
 * @return Received packet type
 * @see xnmeaPACKTYPE
 */
int xnmea_parser_peek(xnmeaPARSER *parser, void **pack_ptr)
{
    int retval = NON;
    xnmeaParserNODE *node = (xnmeaParserNODE *)parser->top_node;

    XNMEA_ASSERT(parser && parser->buffer);

    if(node)
    {
        *pack_ptr = node->pack;
        retval = node->packType;
    }

    return retval;
}

/**
 * \brief Delete top packet from parser
 * @return Deleted packet type
 * @see nmeaPACKTYPE
 */
int xnmea_parser_drop(xnmeaPARSER *parser)
{
    int retval = NON;
    xnmeaParserNODE *node = (xnmeaParserNODE *)parser->top_node;

    XNMEA_ASSERT(parser && parser->buffer);

    if(node)
    {
        if(node->pack)
            free(node->pack);
        retval = node->packType;
        parser->top_node = node->next_node;
        if(!parser->top_node)
            parser->end_node = 0;
        free(node);
    }

    return retval;
}

/**
 * \brief Clear cache of parser
 * @return true (1) - success
 */
int xnmea_parser_buff_clear(xnmeaPARSER *parser)
{
    XNMEA_ASSERT(parser && parser->buffer);
    parser->buff_use = 0;
    return 1;
}

/**
 * \brief Clear packets queue into parser
 * @return true (1) - success
 */
int xnmea_parser_queue_clear(xnmeaPARSER *parser)
{
    XNMEA_ASSERT(parser);
    while(parser->top_node)
        xnmea_parser_drop(parser);
    return 1;
}
