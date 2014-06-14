/*
 * main.cpp
 *
 *  Created on: May 19, 2014
 *      Author: Pimenta
 */

#include "MIPS.hpp"
#include "IOController.hpp"

int main(int argc, char* argv[]) {
  
  MIPS mips("mips");
  IOController ioController("ioController");
  sc_clock clk;
  
  mips.clk(clk);
  mips.ioController(ioController);
  
  sc_start();
  
  return 0;
}
