/*
 * main.cpp
 *
 *  Created on: May 19, 2014
 *      Author: Pimenta
 */

#include "IOController.hpp"
#include "Thread.hpp"

SC_MODULE(Test) {
  sc_in<bool> clk;
  sc_port<readwrite_if> ioController;
  SC_CTOR(Test) {
    SC_METHOD(exec);
    sensitive << clk.pos();
  }
  
  void exec() {
    if (IOController::quit())
      exit(0);
    static int x = 0, y = 0;
    ioController->write(8, 'a');
    int tmp = ioController->read(12);
    if (tmp == 0 && x < 512) {
      ioController->write(0, int(256*x/512.0) << 8);
      ioController->write(4, 0x10010000 + (y*512 + x)*4);
      x++; y++;
    }
    Thread::sleep(17);
  }
};

int main(int argc, char* argv[]) {
  
  Test test("test");
  IOController ioController("ioController");
  sc_clock clk;
  
  test.clk(clk);
  test.ioController(ioController);
  
  sc_start();
  
  
  return 0;
}
