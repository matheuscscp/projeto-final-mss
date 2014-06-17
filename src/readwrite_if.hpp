/*
 * readwrite_if.hpp
 *
 *  Created on: Jun 14, 2014
 *      Author: Pimenta
 */

#ifndef READWRITE_IF_HPP_
#define READWRITE_IF_HPP_

// lib
#include <systemc.h>

struct readwrite_if : public sc_module, public sc_interface {
  virtual uint32_t read(uint32_t addr) = 0;
  virtual void write(uint32_t addr, uint32_t data) = 0;
};

#endif /* READWRITE_IF_HPP_ */
