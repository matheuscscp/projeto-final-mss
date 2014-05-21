/*
 * Context.hpp
 *
 *  Created on: May 19, 2014
 *      Author: Pimenta
 */

#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

// standard
#include <map>
#include <string>

// lib
#include <SDL.h>

class Context {
  public:
    enum InputState {
      PRESSED = 0,
      JUST_PRESSED,
      RELEASED,
      JUST_RELEASED
    };
  private:
    static std::map<SDL_Keycode, std::pair<bool, InputState>> keys;
    static InputState buttons[6];
    static int mousex, mousey, mousedownx, mousedowny, mouseupx, mouseupy;
    static SDL_Window* window;
    static SDL_Renderer* renderer;
    static bool quit;
    static SDL_Texture* texture;
    static uint32_t* pixels;
    static int windowWidth, windowHeight;
  public:
    static void init(const char* title, int w, int h);
    static void close();
    static bool shouldQuit();
    static void input();
    static void render();
    static uint32_t getPixel(uint32_t position);
    static void setPixel(uint32_t pixel, uint32_t position);
    static InputState key(SDL_Keycode keycode);
    static InputState button(int butt);
    static int getMouse();
    static int getMouseDown();
    static int getMouseUp();
};

#endif /* CONTEXT_HPP_ */
