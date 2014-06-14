/*
 * IOController.hpp
 *
 *  Created on: Jun 11, 2014
 *      Author: Pimenta
 */

#ifndef IOCONTROLLER_HPP_
#define IOCONTROLLER_HPP_

// lib
#include <systemc.h>

SC_MODULE(IOController) {
  bool quit, ready2close;
  sc_out<uint32_t> addrRangeSize;
  
  SC_CTOR(IOController);
  ~IOController();
  
  uint32_t read(uint32_t addr);
  void write(uint32_t addr, uint32_t data);
};

#endif /* IOCONTROLLER_HPP_ */
