/*
 * halftone1.cpp
 *
 *  Created on: 03/07/2014
 *      Author: Sergio Cruz
 */

// Libs
#include "halftone1.hpp"

Halftone::Halftone(sc_core::sc_module_name, uint32_t start_addr) :
readwrite_if(start_addr)
{
     flag_rdy = 0;
     hlf = 0;
     start = 0;
     w_im = 512;
     h_im = 512;
     tam_cabecalho = 14 + 40 + 1024;
     tam_img = 0;

}

/*
    Memory Map

    -- Write
    0xFF400200: Image height
    0xFF400204: Imagem width
    0xFF400208: Halftone technique to be applied (0 - 3)
    0xFF40020C: Start
    0xFF400210: Unused
    0xFF400214: Unused
    0xFF400218: Unused
    0xFF40021C: Unused
    0xFF400220: Unused
    0xFF400224: Unused
    0xFF400228: Unused

    -- Read
    0xFF400300: ready ñ Halftone ready flag
    0xFF500000 ñ 0xFF8FFFFC: Image (512 x 512 - Bitmap ".bmp" - 256 colors - 1 byte/pixel)


 */

uint32_t Halftone::size()
{
    return 0x004FFDFC;
}

void Halftone::read(uint32_t src, uint32_t bytes, void* dst)
{
    src -= start_addr;
    uint32_t* ptr = (uint32_t*)dst;
    uint32_t i = 0;

    switch (src)
    {
        case 0xFF400300:
            *ptr = flag_rdy;	break;
        case 0xFF500000:
            for (i = 0; i < bytes; i++)
                *ptr = buff_img[i];
            break;
        default:
            *ptr = 0;
    }
}

void Halftone::write(uint32_t dst, uint32_t bytes, void* src)
{
    dst -= start_addr;
    uint32_t i = 0;

    switch (dst)
    {
        case 0xFF400200:
            h_im = *((uint32_t*)src);
            break;
        case 0xFF400204:
            w_im = *((uint32_t*)src);
            break;
        case 0xFF400208:
            hlf = *((uint32_t*)src);
            break;
        case 0xFF40020C: // Start
            // Halftone Technique
            flag_rdy = 0;
            tam_img = (h_im*w_im) + tam_cabecalho;
            switch (hlf)
            {
                case 0 : halftone0(buff_img, tam_cabecalho, tam_img); break;
                case 1 : halftone1(buff_img, w_im, h_im, tam_cabecalho); break;
                case 2 : halftone2(buff_img, w_im, h_im, tam_cabecalho); break;
                case 3 : halftone3(buff_img, w_im, h_im, tam_cabecalho); break;
                default: halftone1(buff_img, w_im, h_im, tam_cabecalho); break;
            };
            flag_rdy = 1;
            break;
        case 0xFF500000:
            for (i = 0; i < bytes; i++)
                buff_img[i] = *((uint32_t*)src);
            break;
    }
}

