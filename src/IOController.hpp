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
  public:
    typedef IOController SC_CURRENT_USER_MODULE;
    IOController(sc_core::sc_module_name, uint32_t start_addr);
    ~IOController();
    
    uint32_t size();
    
    void read(uint32_t src, uint32_t bytes, void* dst);
    void write(uint32_t dst, uint32_t bytes, void* src);
};

#endif /* IOCONTROLLER_HPP_ */
