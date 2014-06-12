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
    if (Context::button(0)) {
      int x = Context::getMouse() >> 16, y = Context::getMouse() & 0xFFFF;
      Context::setPixel(0xFFFFFF, 0x10010000 + (y*512 + x)*4);
    }
    SDL_Delay(16);
  }
  
  return 0;
}
