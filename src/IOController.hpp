/*
 * IOController.hpp
 *
 *  Created on: Jun 11, 2014
 *      Author: Pimenta
 */

#ifndef IOCONTROLLER_HPP_
#define IOCONTROLLER_HPP_

// local
#include "readwrite_if.hpp"

struct IOController : public readwrite_if {
  SC_CTOR(IOController);
  ~IOController();
  
  uint32_t addrRangeSize();
  uint32_t read(uint32_t addr);
  void write(uint32_t addr, uint32_t data);
};

#endif /* IOCONTROLLER_HPP_ */
