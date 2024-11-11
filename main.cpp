#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>

#include <cstdio>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;

int main(int, char **) {
  // Initialize SDL
  SDL_Init(SDL_INIT_VIDEO);

  // Create a window and an OpenGL context
  SDL_Window *window =
      SDL_CreateWindow("pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
  SDL_GLContext context = SDL_GL_CreateContext(window);

  // Set up the OpenGL viewport
  glViewport(0, 0, 800, 600);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Game loop
  bool running = true;
  while (running) {
    // Handle events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a white square
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(-0.1, -0.1);
    glVertex2f(-0.1, 0.1);
    glVertex2f(0.1, 0.1);
    glVertex2f(0.1, -0.1);
    glEnd();

    // Update the display
    SDL_GL_SwapWindow(window);
  }

  // Clean up
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
