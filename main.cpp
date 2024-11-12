#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>

int gScreenWidth = 1280;
int gScreenHeight = 1024;
bool gIsFullscreen = false;

SDL_Window *gWindow = nullptr;
SDL_Surface *gScreenSurface = nullptr;
SDL_GLContext gContext = NULL;

void getOpenGLInfo() {
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << std::endl;
};

void init() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 24);

  gWindow =
      SDL_CreateWindow("pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       gScreenWidth, gScreenHeight,
                       SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL |
                           (gIsFullscreen * SDL_WINDOW_FULLSCREEN));
  if (gWindow == NULL) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to create OpenGL window: %s\n", SDL_GetError());
    exit(1);
  }

  gContext = SDL_GL_CreateContext(gWindow);
  if (gContext == NULL) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to create OpenGL context: %s\n", SDL_GetError());
    exit(1);
  }

  getOpenGLInfo();
};

void loop() {
  SDL_Event Event;
  bool bQuit = false;

  while (!bQuit) {
    while (SDL_PollEvent(&Event)) {
      if (Event.type == SDL_QUIT)
        bQuit = true;
      else if (Event.type == SDL_KEYDOWN) {
        if (Event.key.keysym.sym == SDLK_ESCAPE)
          bQuit = true;
      }
    }

    // Render the scene
    // GL_Render();

    // Swap the back-buffer and present it
    SDL_GL_SwapWindow(gWindow);
  }
};

void cleanup() {
  SDL_GL_DeleteContext(gContext);
  SDL_DestroyWindow(gWindow);
  SDL_Quit();
};

int main(int, char **) {
  init();
  loop();
  cleanup();

  return 0;
}
