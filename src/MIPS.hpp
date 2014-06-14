/*
 * MIPS.hpp
 *
 *  Created on: Jun 14, 2014
 *      Author: Pimenta
 */

#ifndef MIPS_HPP_
#define MIPS_HPP_

#include <systemc.h>

SC_MODULE(MIPS) {
  uint32_t breg[32];
  
  SC_CTOR(MIPS) {
    
  }
  
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
