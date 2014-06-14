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

static Thread windowThread([]{});
static uint32_t* mem;
static bool quit;

IOController::IOController(sc_core::sc_module_name) {
  windowThread = Thread([]() {
    Context::init("Modelagem de Sistemas em Silicio - 1/2014", 512, 512);
    while (!quit) {
      Context::input();
      Context::render();
    }
    Context::close();
  });
  windowThread.start();
  while (!Context::ready())
    Thread::sleep(50);
  mem = new uint32_t[addrRangeSize()];
  quit = false;
}

IOController::~IOController() {
  quit = true;
  windowThread.join();
  delete[] mem;
}

// =============================================================================
// memory map
// =============================================================================

// 0x00: setPixel::pixel
// 0x04: setPixel::position
// 0x08: key::keycode
// 0x0C: key::return
// 0x10: getWindowSize::return
// 0x14: getPixel::position
// 0x18: getPixel::return
// 0x1C: button::butt
// 0x20: button::return
// 0x24: getMouse::return
// 0x28: getMouseDown::return
// 0x2C: getMouseUp::return
// 0x30: quitRequested::return

uint32_t IOController::addrRangeSize() {
  return 52;
}

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
      
    case 12:
      mem[addr] = Context::quitRequested();
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
