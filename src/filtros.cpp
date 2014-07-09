/*
 * filtros.cpp
 *
 *  Created on: 25/06/2014
 *      Author: Sergio Cruz
 */

#include <stdio.h>
#include "filtros.hpp"

/******************************************************************
 *						HALFTONE VARIANTS
 *****************************************************************/

void halftone0(uint8_t *imagem_entrada,uint32_t tam_cabecalho, uint32_t tam_img)
{
    printf("Aplicando Halftone0...\n");

    //flag_rdy = 0;

    uint32_t i;

    for (i = tam_cabecalho; i < tam_img; i++)
    {
        if (imagem_entrada[i] > 120)
            imagem_entrada[i] = 255;
        else
            imagem_entrada[i] = 0;
    }

    //flag_rdy = 1;

    printf("Ok!\n");
};

//--------------------------------------------------------------------------------------------------------

void halftone1(uint8_t *img, size_t w, size_t h, uint32_t tam_cabecalho)
{
    printf("Aplicando Halftone1...\n");

    //flag_rdy = 0;

    size_t l;
    size_t c;
    size_t i;

    for (l = 0; l < h; l++)
        for (c = 0; c < w; c++)
        {
            i = (l * w) + c;

            if (img[i+tam_cabecalho] > 100)
                img[i+tam_cabecalho] = 255;
            else
                img[i+tam_cabecalho] = 0;
        }

    //flag_rdy = 1;

    printf("Ok!\n");
}

//--------------------------------------------------------------------------------------------------------

void halftone2(uint8_t *img, size_t w, size_t h, uint32_t tam_cabecalho)
{
    printf("Aplicando Halftone2...\n");

    //flag_rdy = 0;

    uint32_t l;
    uint32_t c;
    uint32_t i;
    int32_t val;

    for (l = 0; l < h - 1; l += 2)
        for (c = 0; c < w - 1; c += 2)
        {
            i = (l * w) + c;

            val  = (int32_t) img[i+tam_cabecalho];
            val += (int32_t) img[i + 1+tam_cabecalho];
            val += (int32_t) img[i + w+tam_cabecalho];
            val += (int32_t) img[i + w + 1+tam_cabecalho];

            /*if (val > 512) img[i+tam_cabecalho] = 255;
            else img[i+tam_cabecalho] = 0;
            if (val > 512) img[i+w+tam_cabecalho] = 255;
            else img[i+w+tam_cabecalho] = 0;
            if (val > 512) img[i+w+1+tam_cabecalho] = 255;
            else img[i+w+1+tam_cabecalho] = 0;
            if (val > 512) img[i+1+tam_cabecalho] = 255;
            else img[i+1+tam_cabecalho] = 0;*/

            if (val > (1 * 1024) / 8)
                img[i+tam_cabecalho] = 255;
            else
                img[i+tam_cabecalho] = 0;

            if (val > (3 * 1024) / 8)
                img[i + w + 1+tam_cabecalho] = 255;
            else
                img[i + w + 1+tam_cabecalho] = 0;

            if (val > (5 * 1024) / 8)
                img[i + w+tam_cabecalho] = 255;
            else
                img[i + w+tam_cabecalho] = 0;

            if (val > (7 * 1024) / 8)
                img[i + 1+tam_cabecalho] = 255;
            else
                img[i + 1+tam_cabecalho] = 0;
        }

    //flag_rdy = 1;

    printf("Ok!\n");
}

//--------------------------------------------------------------------------------------------------------

void halftone3(uint8_t *img, size_t w, size_t h, uint32_t tam_cabecalho)
{
    printf("Aplicando Halftone3...\n");

    //flag_rdy = 0;

    uint32_t l;
    uint32_t c;
    uint32_t i;
    int32_t pe;     /* propagated error */
    int32_t te;     /* total error */

    pe = 0;
    te = 0;

    for (l = 1; l < h; l++)
        for (c = 1; c < w; c++)
        {
            i = (l * w) + c;
            te = (int32_t) img[i+tam_cabecalho] + pe;

            if (te > 127)
                img[i+tam_cabecalho] = 255;
            else
                img[i+tam_cabecalho] = 0;

            pe = te - img[i+tam_cabecalho];
        }

    //flag_rdy = 1;

    printf("Ok!\n");
}

