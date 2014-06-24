/*
 * readwrite_if.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: Pimenta
 */

// this
#include "readwrite_if.hpp"

readwrite_if::readwrite_if(uint32_t start_addr) : start_addr(start_addr) {
  
}

uint32_t readwrite_if::start_address() {
  return start_addr;
}
