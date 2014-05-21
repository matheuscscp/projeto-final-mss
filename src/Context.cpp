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
#include <SDL_image.h>

using namespace std;

// =============================================================================
// Context
// =============================================================================

map<SDL_Keycode, pair<bool, Context::InputState>> Context::keys;
Context::InputState Context::buttons[6];
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
int Context::windowWidth;
int Context::windowHeight;

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
    butt = RELEASED;
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, w, h);
  pixels = new uint32_t[w*h];
  memset(pixels, 0, w*h*sizeof(uint32_t));
  windowWidth = w;
  windowHeight = h;
}

void Context::close() {
  delete[] pixels;
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
}

bool Context::shouldQuit() {
  return quit;
}

void Context::input() {
  for (auto& kv : keys) {
    if (kv.second.second == JUST_PRESSED)
      kv.second.second = PRESSED;
    else if (kv.second.second == JUST_RELEASED)
      kv.second.second = RELEASED;
  }
  
  SDL_GetMouseState(&mousex, &mousey);
  for (auto& i : buttons) {
    if (i == JUST_PRESSED)
      i = PRESSED;
    else if (i == JUST_RELEASED)
      i = RELEASED;
  }
  
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        if (!keys[event.key.keysym.sym].first) {
          keys[event.key.keysym.sym].first = true;
          keys[event.key.keysym.sym].second = JUST_PRESSED;
        }
        break;
        
      case SDL_KEYUP:
        keys[event.key.keysym.sym].first = false;
        keys[event.key.keysym.sym].second = JUST_RELEASED;
        break;
        
      case SDL_MOUSEBUTTONDOWN:
        buttons[event.button.button] = JUST_PRESSED;
        mousedownx = mousex;
        mousedowny = mousey;
        break;
        
      case SDL_MOUSEBUTTONUP:
        buttons[event.button.button] = JUST_RELEASED;
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

void Context::setPixel(uint32_t pixel, uint32_t position) {
  pixels[(position - 0x10010000)/4] = pixel;
}

uint32_t Context::getPixel(uint32_t position) {
  return pixels[(position - 0x10010000)/4];
}

Context::InputState Context::key(SDL_Keycode keycode) {
  if (keys.find(keycode) == keys.end()) {
    keys[keycode].first = false;
    keys[keycode].second = RELEASED;
    return RELEASED;
  }
  return keys[keycode].second;
}

Context::InputState Context::button(int butt) {
  return buttons[butt];
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
