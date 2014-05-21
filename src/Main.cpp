/*
 * main.cpp
 *
 *  Created on: May 19, 2014
 *      Author: Pimenta
 */

#include "Context.hpp"

int main(int argc, char* argv[]) {
  
  Context::init("MSS", 512, 512);

  Context::setPixel(0xFFFFFF, 0x10010000);
  Context::setPixel(0xFFFFFF, 0x10010000 + 2048);
  Context::setPixel(0xFFFFFF, 0x10010000 + 4096);
  Context::setPixel(0xFFFFFF, 0x10010000 + 12);
  Context::setPixel(0xFFFFFF, 0x10010000 + 16);
  Context::setPixel(0xFFFFFF, 0x10010000 + 20);
  Context::setPixel(0xFFFFFF, 0x10010000 + 24);
  Context::setPixel(0xFFFFFF, 0x10010000 + 28);
  
  while (!Context::shouldQuit()) {
    Context::input();
    
    Context::render();
  }
  
  Context::close();
  
  return 0;
}
