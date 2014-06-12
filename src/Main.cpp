/*
 * main.cpp
 *
 *  Created on: May 19, 2014
 *      Author: Pimenta
 */

#include "IOController.hpp"

int main(int argc, char* argv[]) {
  
  IOController ioController("ioController");
  
  while (!ioController.quit) {
    Thread::sleep(50);
  }
  
  return 0;
}
