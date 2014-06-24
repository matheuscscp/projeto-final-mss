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

IOController::IOController(sc_core::sc_module_name, uint32_t start_addr) :
readwrite_if(start_addr) {
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
// 0x00400114: 4-byte ascii code (reading from this address will
//             suspend the thread until the user press a key)

// attention: everything, except video buffer, is read-only

uint32_t IOController::size() {
  return 0x00400118;
}

void IOController::read(uint32_t src, uint32_t bytes, void* dst) {
  src -= start_addr;
  uint32_t* ptr = (uint32_t*)dst;
  switch (src) {
    case 0x00400100:  *ptr = Context::button(0);        break;
    case 0x00400101:  *ptr = Context::button(1);        break;
    case 0x00400102:  *ptr = Context::quitRequested();  break;
    case 0x00400104:  *ptr = Context::getWindowSize();  break;
    case 0x00400108:  *ptr = Context::getMouse();       break;
    case 0x0040010C:  *ptr = Context::getMouseDown();   break;
    case 0x00400110:  *ptr = Context::getMouseUp();     break;
    case 0x00400114:  *ptr = Context::readKey();        break;
    default:
      if (src < 0x00400000) *ptr = Context::getPixel(src);
      if (src < 0x00400100) *ptr = Context::key(src - 0x00400000);
  }
}

void IOController::write(uint32_t dst, uint32_t bytes, void* src) {
  dst -= start_addr;
  if (dst < 0x00400000) Context::setPixel(*((uint32_t*)src), dst);
}
