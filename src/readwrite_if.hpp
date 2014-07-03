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
  protected:
    uint32_t start_addr;
  public:
    readwrite_if(uint32_t start_addr);
    
    uint32_t start_address();
    virtual uint32_t size() = 0;
    
    virtual void read(uint32_t src, uint32_t bytes, void* dst) = 0;
    virtual void write(uint32_t dst, uint32_t bytes, void* src) = 0;
};

#endif /* READWRITE_IF_HPP_ */
