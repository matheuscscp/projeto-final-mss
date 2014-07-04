#ifndef MASTER_READWRITE_IF_HPP_
#define MASTER_READWRITE_IF_HPP_

#include <systemc.h>

class master_readwrite_if
  : public sc_interface
{
public:
    virtual void read(unsigned int unique_priority, uint32_t src, uint32_t bytes, void* dst) = 0;
    virtual void write(unsigned int unique_priority, uint32_t dst, uint32_t bytes, void* src) = 0;

};

#endif
