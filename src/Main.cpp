/*
 * main.cpp
 *
 *  Created on: May 19, 2014
 *      Author: Pimenta
 */

#include "MIPS.hpp"
#include "IOController.hpp"
#include "bus.h"

int main(int argc, char* argv[]) {

    MIPS mips("mips");
    IOController ioController("ioController", 0xFF000000);
    simple_bus bus("bus");
    sc_clock clk;

    mips.clk(clk);
    //  mips.ioController(ioController);
    mips.ioController(bus);

    bus.clock(clk);
    bus.slave_port(ioController);

    sc_start();

    return 0;
}
