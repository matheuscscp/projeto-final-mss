/*
 * main.cpp
 *
 *  Created on: May 19, 2014
 *      Author: Pimenta
 */

#include "MIPS.hpp"
#include "IOController.hpp"
#include "bus.h"

int sc_main(int argc, char* argv[]) {

    MIPS mips("mips");
    IOController ioController("ioController", 0xFF000000);
    simple_bus bus("bus");
    sc_clock clk;
 //sc_clock clk("_Clock", 10, SC_NS,0.5, 1, SC_NS);



    mips.clk(clk);
      mips.bus_port(bus);
    //mips.ioController(bus);

    bus.clock(clk);
    bus.slave_port(ioController);

    sc_start();

    return 0;
}
