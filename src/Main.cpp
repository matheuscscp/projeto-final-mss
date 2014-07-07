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
//    IOController ioController_mesmo_mapeamento("ioController_mesmo_mapeamento", 0xFF000000);
    simple_bus *bus;

    bus = new simple_bus ("bus");
    sc_clock clk;

    mips.clk(clk);
    mips.bus_port(*bus);

    bus->clock(clk);
    bus->slave_port(ioController);
//    bus->slave_port(ioController_mesmo_mapeamento);

    sc_start();

    return 0;
}
