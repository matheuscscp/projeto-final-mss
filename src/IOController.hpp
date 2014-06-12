/*
 * IOController.hpp
 *
 *  Created on: Jun 11, 2014
 *      Author: Pimenta
 */

#ifndef IOCONTROLLER_HPP_
#define IOCONTROLLER_HPP_

// standard
#include <map>

// lib
#include <systemc.h>

// local
#include "Thread.hpp"
#include "Context.hpp"

using namespace std;

SC_MODULE(IOController) {
  bool quit;
private:
  bool contextReady;
  Thread windowThread;
  map<SDL_threadID, bool> contextClients;
public:
  SC_CTOR(IOController) : quit(false), contextReady(false), windowThread([&]() {
    Context::init("MSS", 512, 512);
    contextReady = true;
    while (!quit && !Context::shouldQuit()) {
      Context::input();
      Context::render();
    }
    quit = true;
    while (true) {
      unsigned acked = 0;
      for (auto& kv : contextClients) {
        if (kv.second)
          acked++;
      }
      if (acked == contextClients.size())
        break;
      Thread::sleep(50);
    }
    Context::close();
  }) {
    windowThread.start();
  }
  
  void registerContextClient() {
    SDL_threadID id = SDL_ThreadID();
    if (contextClients.find(id) == contextClients.end())
      contextClients[id] = false;
    while (!contextReady)
      Thread::sleep(50);
  }
  
  void unregisterContextClient() {
    SDL_threadID id = SDL_ThreadID();
    if (contextClients.find(id) != contextClients.end())
      contextClients[id] = true;
    windowThread.join();
  }
};

#endif /* IOCONTROLLER_HPP_ */
