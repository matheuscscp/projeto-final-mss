/*
 * IOController.hpp
 *
 *  Created on: Jun 11, 2014
 *      Author: Pimenta
 */

#ifndef IOCONTROLLER_HPP_
#define IOCONTROLLER_HPP_

// lib
#include <systemc.h>

// this
#include "Thread.hpp"
#include "Context.hpp"

SC_MODULE(IOController) {
  bool quit;
  Thread windowThread;
  
  SC_CTOR(IOController) : quit(false), windowThread([&]() {
    Context::init("MSS", 512, 512);
    while (!quit && !Context::shouldQuit()) {
      Context::input();
      Context::render();
    }
    Context::close();
    quit = true;
  }) {
    windowThread.start();
  }
};

#endif /* IOCONTROLLER_HPP_ */
