/*
 * main.cpp
 *
 *  Created on: May 19, 2014
 *      Author: Pimenta
 */

#include "IOController.hpp"

int main(int argc, char* argv[]) {
  
  IOController ioController("ioController");
  
  ioController.waitContext();
  
  return 0;
}
