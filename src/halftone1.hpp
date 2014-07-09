/*
 * halftone1.h
 *
 *  Created on: 03/07/2014
 *      Author: Sergio Cruz
 */

#ifndef HALFTONE1_H_
#define HALFTONE1_H_

// local
#include "readwrite_if.hpp"
#include "filtros.hpp"


struct Halftone : public readwrite_if
{
    public:
        typedef Halftone SC_CURRENT_USER_MODULE;
        Halftone(sc_core::sc_module_name, uint32_t start_addr);

        uint32_t size();

        void read(uint32_t src, uint32_t bytes, void* dst);
        void write(uint32_t dst, uint32_t bytes, void* src);

        uint32_t flag_rdy;
        uint32_t hlf;
        uint32_t start;
        uint32_t w_im;
        uint32_t h_im;
        uint32_t tam_cabecalho;
        uint32_t tam_img;
        uint8_t buff_img[0x40000];

};


#endif /* HALFTONE1_H_ */
