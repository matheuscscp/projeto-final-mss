/*
 * IOController.cpp
 *
 *  Created on: Jun 13, 2014
 *      Author: Pimenta
 */

// this
#include "IOController.hpp"

// local
#include "Context.hpp"
#include "Thread.hpp"

#define ADDR_RANGE_SIZE 48

static Thread windowThread([]{});
static uint32_t mem[ADDR_RANGE_SIZE >> 2];

IOController::IOController(sc_core::sc_module_name) :
quit(false), ready2close(false) {
  windowThread = Thread([&]() {
    Context::init("Modelagem de Sistemas em Silicio - 1/2014", 512, 512);
    while (!quit && !Context::quitRequested()) {
      Context::input();
      Context::render();
    }
    quit = true;
    while (!ready2close)
      Thread::sleep(50);
    Context::close();
  });
  windowThread.start();
  while (!Context::ready())
    Thread::sleep(50);
  addrRangeSize.initialize(ADDR_RANGE_SIZE);
}

IOController::~IOController() {
  ready2close = true;
  windowThread.join();
}

// =============================================================================
// memory map
// =============================================================================

//  0: setPixel::pixel
//  1: setPixel::position
//  2: key::keycode
//  3: key::return
//  4: getWindowSize::return
//  5: getPixel::position
//  6: getPixel::return
//  7: button::butt
//  8: button::return
//  9: getMouse::return
// 10: getMouseDown::return
// 11: getMouseUp::return

uint32_t IOController::read(uint32_t addr) {
  addr >>= 2;
  switch (addr) {
    case 3:
      mem[addr] = Context::key(mem[addr - 1]);
      break;
      
    case 4:
      mem[addr] = Context::getWindowSize();
      break;
      
    case 6:
      mem[addr] = Context::getPixel(mem[addr - 1]);
      break;
      
    case 8:
      mem[addr] = Context::button(mem[addr - 1]);
      break;
      
    case 9:
      mem[addr] = Context::getMouse();
      break;
      
    case 10:
      mem[addr] = Context::getMouseDown();
      break;
      
    case 11:
      mem[addr] = Context::getMouseUp();
      break;
      
    default:
      break;
  }
  return mem[addr];
}

void IOController::write(uint32_t addr, uint32_t data) {
  addr >>= 2;
  mem[addr] = data;
  switch (addr) {
    case 1:
      Context::setPixel(mem[addr - 1], mem[addr]);
      break;
      
    default:
      break;
  }
}
