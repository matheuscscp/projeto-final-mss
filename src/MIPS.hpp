/*
 * MIPS.hpp
 *
 *  Created on: Jun 14, 2014
 *      Author: Pimenta
 */

#ifndef MIPS_HPP_
#define MIPS_HPP_

// lib
#include <systemc.h>

// local
#include "Context.hpp"
#include "Thread.hpp"
//#include "readwrite_if.hpp"
#include "master_readwrite_if.hpp"

//Prioridade do MIPS (1 = maxima)
unsigned int m_unique_priority = 1;

struct Bitmap {
    short magic_number;
    int file_size;
    int reserved;
    int header_size;
    int data_distance;
    int width;
    int height;
    short planes;
    short bpp;
    int compression;
    int data_size;
    int horizontal_resolution;
    int vertical_resolution;
    int palette_size;
    int important_colors;
    uint32_t* buf;
};

SC_MODULE(MIPS) {
    sc_in<bool> clk;
    sc_port<master_readwrite_if> bus_port;

    uint32_t breg[32];

    Bitmap bg;

    SC_CTOR(MIPS) {
        SC_THREAD(exec);
        sensitive << clk.pos();

        const char fn[] = "teste.bmp";
        const char mode[] = "rb";

        breg[4] = (uint32_t)fn; breg[5] = (uint32_t)mode;
        fileOpen();
        breg[4] = breg[2];

        breg[5] = (uint32_t)&bg.magic_number; breg[6] = 2;
        fileRead();
        breg[5] = (uint32_t)&bg.file_size; breg[6] = 4;
        fileRead();
        breg[5] = (uint32_t)&bg.reserved; breg[6] = 4;
        fileRead();
        breg[5] = (uint32_t)&bg.header_size; breg[6] = 4;
        fileRead();
        breg[5] = (uint32_t)&bg.data_distance; breg[6] = 4;
        fileRead();
        breg[5] = (uint32_t)&bg.width; breg[6] = 4;
        fileRead();
        breg[5] = (uint32_t)&bg.height; breg[6] = 4;
        fileRead();
        breg[5] = (uint32_t)&bg.planes; breg[6] = 2;
        fileRead();
        breg[5] = (uint32_t)&bg.bpp; breg[6] = 2;
        fileRead();
        breg[5] = (uint32_t)&bg.compression; breg[6] = 4;
        fileRead();
        breg[5] = (uint32_t)&bg.data_size; breg[6] = 4;
        fileRead();
        breg[5] = (uint32_t)&bg.horizontal_resolution; breg[6] = 4;
        fileRead();
        breg[5] = (uint32_t)&bg.vertical_resolution; breg[6] = 4;
        fileRead();
        breg[5] = (uint32_t)&bg.palette_size; breg[6] = 4;
        fileRead();
        breg[5] = (uint32_t)&bg.important_colors; breg[6] = 4;
        fileRead();

        bg.buf = new uint32_t[bg.width*bg.height];
        breg[5] = (uint32_t)bg.buf; breg[6] = bg.width*bg.height*bg.bpp/8;
        fileRead();

        fileClose();

    }

    ~MIPS() {
        delete[] bg.buf;
    }

    void exec() {
        uint32_t ioWord;

        bus_status current_bus_staus;

        static bool inited = false;
        if (!inited) {
            inited = true;
            //Verifica se barramento esta livre
            while(bus_port->get_status(m_unique_priority) != BUS_OK)
                wait();
            bus_port->read(1, 0xFF400104, 4, &ioWord);
            int w = ioWord >> 16, h = ioWord & 0xFFFF;

            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++){
                    //Verifica se barramento esta livre
                    while(bus_port->get_status(m_unique_priority) != BUS_OK)
                        wait();
                    bus_port->write(m_unique_priority, 0xFF000000 + ((y*w + x) << 2), 4, &bg.buf[(bg.height - 1 - y)*bg.width + x]);
                }
            }
        }

        while(true){
            //Verifica se barramento esta livre
            while(bus_port->get_status(m_unique_priority) != BUS_OK)
                wait();
            bus_port->read(m_unique_priority, 0xFF400000 + 'w', 4, &ioWord);
            if (ioWord == 1) {
                printf("Insert a string: ");
                fflush(stdout);
                std::string tmp;
                //Verifica se barramento esta livre
                while(bus_port->get_status(m_unique_priority) != BUS_OK)
                    wait();
                bus_port->read(m_unique_priority, 0xFF400114, 4, &ioWord);
                while (ioWord != '\r') {
                    tmp += char(ioWord);
                    printf("%c", char(ioWord));
                    fflush(stdout);
                    //Verifica se barramento esta livre
                    while(bus_port->get_status(m_unique_priority) != BUS_OK)
                        wait();
                    bus_port->read(m_unique_priority, 0xFF400114, 4, &ioWord);
                }
                printf("\nString inserted: %s\n", tmp.c_str());
                fflush(stdout);
            }

            //Verifica se barramento esta livre
            while(bus_port->get_status(m_unique_priority) != BUS_OK)
                wait();

            bus_port->read(m_unique_priority, 0xFF400102, 4, &ioWord);
            if (ioWord){
                exit();
                break;
            }

            breg[4] = 33;
            sleep();
        }
    }
    // ===========================================================================
    // syscalls (code in v0)
    // ===========================================================================
    // v0: 2
    // a0: 4
    // a1: 5
    // a2: 6
    void exit() { // v0 = 10
        sc_stop();
    }
    void fileOpen() { // v0 = 13
        breg[2] = (uint32_t)fopen((const char*)breg[4], (const char*)breg[5]);
    }
    void fileRead() { // v0 = 14
        breg[2] = (uint32_t)fread((void*)breg[5], breg[6], 1, (FILE*)breg[4]);
    }
    void fileWrite() { // v0 = 15
        breg[2] = (uint32_t)fwrite((const void*)breg[5], breg[6], 1, (FILE*)breg[4]);
    }
    void fileClose() { // v0 = 16
        fclose((FILE*)breg[4]);
    }
    void sleep() { // v0 = 32
        Thread::sleep(breg[4]);
    }
};

#endif /* MIPS_HPP_ */
