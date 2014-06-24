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

// lib
#include <SDL.h>

class Context {
  private:
    static std::map<SDL_Keycode, bool> keys;
    static bool buttons[6];
    static int mousex, mousey, mousedownx, mousedowny, mouseupx, mouseupy;
    static SDL_Window* window;
    static SDL_Renderer* renderer;
    static bool quit;
    static SDL_Texture* texture;
    static uint32_t* pixels;
    static uint32_t windowWidth, windowHeight;
    static bool isReady;
    static uint32_t keyDown;
  public:
    static void init(const char* title, int w, int h);
    static void close();
    static bool ready();
    static void input();
    static void render();
    
    static int getWindowSize();
    static uint32_t getPixel(uint32_t position);
    static void setPixel(uint32_t pixel, uint32_t position);
    static bool key(SDL_Keycode keycode);
    static bool button(int butt);
    static int getMouse();
    static int getMouseDown();
    static int getMouseUp();
    static bool quitRequested();
    static uint32_t readKey();
};

#endif /* CONTEXT_HPP_ */
