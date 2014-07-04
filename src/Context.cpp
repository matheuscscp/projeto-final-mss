/*
 * Context.cpp
 *
 *  Created on: May 19, 2014
 *      Author: Pimenta
 */

// this
#include "Context.hpp"

// standard
#include <cstdio>
#include <cstdlib>
#include <cstring>

// lib
#include <SDL2/SDL_image.h>

// local
#include "Thread.hpp"

using namespace std;

// =============================================================================
// Context
// =============================================================================

map<SDL_Keycode, bool> Context::keys;
bool Context::buttons[6];
int Context::mousex;
int Context::mousey;
int Context::mousedownx = -1;
int Context::mousedowny = -1;
int Context::mouseupx = -1;
int Context::mouseupy = -1;
SDL_Window* Context::window = nullptr;
SDL_Renderer* Context::renderer = nullptr;
bool Context::quit = false;
uint32_t* Context::pixels = nullptr;
SDL_Texture* Context::texture = nullptr;
uint32_t Context::windowWidth;
uint32_t Context::windowHeight;
bool Context::isReady = false;
uint32_t Context::keyDown = 0;
bool Context::keyDownFlipFlop = false;

void Context::init(const char* title, int w, int h) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
    fprintf(stderr, "SDL_Init failure: %s\n", SDL_GetError());
    exit(0);
  }
  if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) != (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF)) {
    fprintf(stderr, "IMG_Init failure\n");
    exit(0);
  }
  window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  for (auto& butt : buttons)
    butt = false;
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, w, h);
  pixels = new uint32_t[w*h];
  memset(pixels, 0, w*h*sizeof(uint32_t));
  windowWidth = w;
  windowHeight = h;
  isReady = true;
}

void Context::close() {
  isReady = false;
  delete[] pixels;
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
}

bool Context::ready() {
  return isReady;
}

void Context::input() {
  SDL_GetMouseState(&mousex, &mousey);
  
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        keys[event.key.keysym.sym] = true;
        keyDown = event.key.keysym.sym;
        keyDownFlipFlop = !keyDownFlipFlop;
        break;
        
      case SDL_KEYUP:
        keys[event.key.keysym.sym] = false;
        break;
        
      case SDL_MOUSEBUTTONDOWN:
        buttons[event.button.button] = true;
        mousedownx = mousex;
        mousedowny = mousey;
        break;
        
      case SDL_MOUSEBUTTONUP:
        buttons[event.button.button] = false;
        mouseupx = mousex;
        mouseupy = mousey;
        break;
        
      case SDL_QUIT:
        quit = true;
        break;
        
      default:
        break;
    }
  }
}

void Context::render() {
  SDL_UpdateTexture(texture, nullptr, pixels, windowWidth*sizeof(uint32_t));
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

int Context::getWindowSize() {
  return (windowWidth << 16) | (windowHeight & 0xFFFF);
}

uint32_t Context::getPixel(uint32_t position) {
  position >>= 2;
  if (position >= windowWidth*windowHeight)
    return 0;
  return pixels[position];
}

void Context::setPixel(uint32_t pixel, uint32_t position) {
  position >>= 2;
  if (position < windowWidth*windowHeight)
    pixels[position] = pixel;
}

bool Context::key(SDL_Keycode keycode) {
  if (keys.find(keycode) == keys.end())
    return false;
  return keys[keycode];
}

bool Context::button(int butt) {
  return butt == 0 ? buttons[SDL_BUTTON_LEFT] : buttons[SDL_BUTTON_RIGHT];
}

int Context::getMouse() {
  return (mousex << 16) | (mousey & 0xFFFF);
}

int Context::getMouseDown() {
  return (mousedownx << 16) | (mousedowny & 0xFFFF);
}

int Context::getMouseUp() {
  return (mouseupx << 16) | (mouseupy & 0xFFFF);
}

bool Context::quitRequested() {
  return quit;
}

uint32_t Context::readKey() {
  bool clk = keyDownFlipFlop;
  while (clk == keyDownFlipFlop)
    Thread::sleep(20);
  return keyDown;
}
