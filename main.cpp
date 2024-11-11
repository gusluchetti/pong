#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <cstdio>
#include <iostream>

#include <SDL2_image/SDL_image.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;

// init sdl and opens window
bool init() {
  bool success = true;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    success = false;
  } else {
    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      success = false;
    } else {
      gScreenSurface = SDL_GetWindowSurface(gWindow);
    }
  }

  return success;
};

// loads media
bool loadMedia() {
  bool success = true;

  gHelloWorld =
      SDL_LoadBMP("02_getting_an_image_on_the_screen/hello_world.bmp");
  if (gHelloWorld == NULL) {
    printf("Unable to load image %s! SDL Error: %s\n",
           "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError());
    success = false;
  }

  return success;
};

// frees media and shuts down
bool close();

int main(int argc, char *argv[]) {
  std::cout << "Hello World!" << std::endl;
  return 0;
}
