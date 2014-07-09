/*
 * filtros.h
 *
 *  Created on: 25/06/2014
 *      Author: Sergio Cruz
 */

#ifndef FILTROS_H_
#define FILTROS_H_

/******************************************************************
 *							DECLARATIONS
 *****************************************************************/
 #include <stdint.h>

//uint32_t flag_rdy = 0;
//uint32_t hlf = 0;
//uint32_t start = 0;
//uint32_t w_im = 512;
//uint32_t h_im = 512;
//uint32_t tam_cabecalho = 14 + 40 + 1024;
//uint32_t tam_img = 0;
//uint8_t buff_img[0x40000];


void halftone0(uint8_t *imagem_entrada,uint32_t tam_cabecalho, uint32_t tam_img);
void halftone1(uint8_t *img, size_t w, size_t h, uint32_t tam_cabecalho);
void halftone2(uint8_t *img, size_t w, size_t h, uint32_t tam_cabecalho);
void halftone3(uint8_t *img, size_t w, size_t h, uint32_t tam_cabecalho);


#endif /* FILTROS_H_ */
