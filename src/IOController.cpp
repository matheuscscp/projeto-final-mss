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
static bool quit;

IOController::IOController(sc_core::sc_module_name) {
  quit = false;
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
}

IOController::~IOController() {
  quit = true;
  windowThread.join();
}

// =============================================================================
// memory map
// =============================================================================

// 0x00000000 - 0x003FFFFC: 4-byte pixel rows, video buffer
//                          (unused[31:24], r[23:16], g[15:8], b[7:0])
// 0x00400000 - 0x004000FF: 1-byte state of the ascii keyboard keys
//                          (zero if not pressed. 0x00400000 for ascii code 0,
//                          0x004000FF for ascii code 255)
// 0x00400100: 1-byte state of the left mouse button (zero if not pressed)
// 0x00400101: 1-byte state of the right mouse button (zero if not pressed)
// 0x00400102: 1-byte (non-zero if the user requested to close the window)
// 0x00400104: 4-byte window size in pixels (w[31:16], h[15:0])
// 0x00400108: 4-byte mouse position in pixels (x[31:16], y[15:0])
// 0x0040010C: 4-byte mouse position in pixels in the last time a mouse button
//             was pressed (x[31:16], y[15:0])
// 0x00400110: 4-byte mouse position in pixels in the last time a mouse button
//             was released (x[31:16], y[15:0])

// attention: everything, except video buffer, is read-only

uint32_t IOController::read(uint32_t addr) {
  switch (addr) {
    case 0x00400100:  return Context::button(0);
    case 0x00400101:  return Context::button(1);
    case 0x00400102:  return Context::quitRequested();
    case 0x00400104:  return Context::getWindowSize();
    case 0x00400108:  return Context::getMouse();
    case 0x0040010C:  return Context::getMouseDown();
    case 0x00400110:  return Context::getMouseUp();
    default:
      if (addr < 0x00400000) return Context::getPixel(addr);
      if (addr < 0x00400100) return Context::key(addr - 0x00400000);
  }
  return 0;
}

void IOController::write(uint32_t addr, uint32_t data) {
  if (addr < 0x00400000) Context::setPixel(data, addr);
}
