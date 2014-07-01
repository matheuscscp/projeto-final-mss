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
  uint16_t unused0;
  uint16_t magic_number;
  uint32_t file_size;
  uint32_t reserved;
  uint32_t header_size;
  uint32_t data_distance;
  uint32_t width;
  uint32_t height;
  uint16_t planes;
  uint16_t bpp;
  uint32_t compression;
  uint32_t data_size;
  uint32_t horizontal_resolution;
  uint32_t vertical_resolution;
  uint32_t palette_size;
  uint32_t important_colors;
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
    
    breg[5] = (uint32_t)&bg.magic_number; breg[6] = 54;
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
    
    static bool inited = false;
    if (!inited) {
      inited = true;
      ioController->read(0xFF400104, 4, &ioWord);
      int w = ioWord >> 16, h = ioWord & 0xFFFF;
      for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++)
          ioController->write(0xFF000000 + ((y*w + x) << 2), 4, &bg.buf[(bg.height - 1 - y)*bg.width + x]);
      }
    }
    
    ioController->read(0xFF400000 + 'w', 4, &ioWord);
    if (ioWord == 1) {
      printf("Insert a string: ");
      fflush(stdout);
      std::string tmp;
      ioController->read(0xFF400114, 4, &ioWord);
      while (ioWord != '\r') {
        tmp += char(ioWord);
        printf("%c", char(ioWord));
        fflush(stdout);
        ioController->read(0xFF400114, 4, &ioWord);
      }
      printf("\nString inserted: %s\n", tmp.c_str());
      fflush(stdout);
    }
    
    ioController->read(0xFF400102, 4, &ioWord);
    if (ioWord)
      exit();
    
    breg[4] = 33;
    sleep();
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
