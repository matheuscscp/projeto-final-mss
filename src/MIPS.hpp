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
#include "readwrite_if.hpp"

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
  sc_port<readwrite_if> ioController;
  
  uint32_t breg[32];
  
  Bitmap bg;
  
  SC_CTOR(MIPS) {
    SC_METHOD(exec);
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
    static bool inited = false;
    if (!inited) {
      inited = true;
      paint();
    }
    if (ioController->read(0x30))
      exit(0);
    Thread::sleep(33);
  }
  
  void paint() {
    int wSize = ioController->read(0x10), w = wSize >> 16, h = wSize & 0xFFFF;
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        ioController->write(0x0, bg.buf[(bg.height - 1 - y)*bg.width + x]);
        ioController->write(0x4, 0x10010000 + (y*w + x)*4);
      }
    }
  }
  
  // ===========================================================================
  // file syscalls
  // ===========================================================================
  // v0 = 2
  // a0 = 4
  // a1 = 5
  // a2 = 6
  void fileOpen() {
    breg[2] = (uint32_t)fopen((const char*)breg[4], (const char*)breg[5]);
  }
  void fileRead() {
    breg[2] = (uint32_t)fread((void*)breg[5], breg[6], 1, (FILE*)breg[4]);
  }
  void fileWrite() {
    breg[2] = (uint32_t)fwrite((const void*)breg[5], breg[6], 1, (FILE*)breg[4]);
  }
  void fileClose() {
    fclose((FILE*)breg[4]);
  }
};

#endif /* MIPS_HPP_ */
