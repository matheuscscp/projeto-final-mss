#ifndef __simple_bus_h
#define __simple_bus_h

#include "systemc.h"

#include "bus_types.h"
#include "bus_request.h"
#include "readwrite_if.hpp"

class simple_bus
  : public sc_module
{
public:
  // ports
  sc_in_clk clock;
  sc_port<readwrite_if, 0> slave_port;

  SC_HAS_PROCESS(simple_bus);

  // constructor
  simple_bus(sc_module_name name_)
    : sc_module(name_)
    , m_current_request(0)
  {
    // process declaration
    SC_METHOD(main_action);
    dont_initialize();
    sensitive << clock.neg();
  }

  // process
  void main_action();

  //  BUS interface - master
  void read(uint32_t src, uint32_t bytes, void* dst);
  void write(uint32_t dst, uint32_t bytes, void* src);

private:
  void handle_request(void);
  void end_of_elaboration(void);
  readwrite_if * get_slave(uint32_t address);
  simple_bus_request * get_request(void);


private:
  simple_bus_request_vec m_requests;
  simple_bus_request *m_current_request;

};

#endif
