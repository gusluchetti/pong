#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>

int gScreenWidth = 1280;
int gScreenHeight = 1024;
bool gIsFullscreen = false;

SDL_Window *gWindow = nullptr;
SDL_Surface *gScreenSurface = nullptr;
SDL_GLContext gContext = NULL;

GLuint gVAO;
GLuint gVBO;
GLuint gMainProgram;

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

void cleanup() {
  SDL_GL_DeleteContext(gContext);
  SDL_DestroyWindow(gWindow);
  SDL_Quit();
};

bool GL_LoadShader(GLuint &uiShader, GLenum ShaderType,
                   const GLchar *p_cShader) {
  // Build and link the shader program
  uiShader = glCreateShader(ShaderType);
  glShaderSource(uiShader, 1, &p_cShader, NULL);
  glCompileShader(uiShader);

  // Check for errors
  GLint iTestReturn;
  glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iTestReturn);
  if (iTestReturn == GL_FALSE) {
    GLchar p_cInfoLog[1024];
    int32_t iErrorLength;
    glGetShaderInfoLog(uiShader, 1024, &iErrorLength, p_cInfoLog);
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to compile shader: %s\n", p_cInfoLog);
    glDeleteShader(uiShader);
    return false;
  }
  return true;
};

bool GL_LoadShaders(GLuint &uiShader, GLuint uiVertexShader,
                    GLuint uiFragmentShader) {
  // Link the shaders
  uiShader = glCreateProgram();
  glAttachShader(uiShader, uiVertexShader);
  glAttachShader(uiShader, uiFragmentShader);
  glLinkProgram(uiShader);

  // Check for error in link
  GLint iTestReturn;
  glGetProgramiv(uiShader, GL_LINK_STATUS, &iTestReturn);
  if (iTestReturn == GL_FALSE) {
    GLchar p_cInfoLog[1024];
    int32_t iErrorLength;
    glGetShaderInfoLog(uiShader, 1024, &iErrorLength, p_cInfoLog);
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to link shaders: %s\n", p_cInfoLog);
    glDeleteProgram(uiShader);
    return false;
  }
  return true;
};

bool GL_Init() {
  glewExperimental = GL_TRUE;
  GLenum GlewError = glewInit();
  if (GlewError != GLEW_OK) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to initialize GLEW: %s\n",
                    glewGetErrorString(GlewError));
    return false;
  }

  // Set up initial GL attributes
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);

  // Create vertex shader source
  const GLchar p_cVertexShaderSource[] = {"#version 430 core\n \
    layout(location = 0) in vec2 v2VertexPos2D;\n \
    void main() \n \
    { gl_Position = vec4(v2VertexPos2D, 0.0f, 1.0f); }"};

  // Create vertex shader
  GLuint uiVertexShader;
  if (!GL_LoadShader(uiVertexShader, GL_VERTEX_SHADER, p_cVertexShaderSource))
    return false;

  // Create fragment shader source
  const GLchar p_cFragmentShaderSource[] = {"#version 430 core\n \
    out vec3 v3FragOutput;\n \
    void main() \n \
    {\n \
        v3FragOutput = vec3(1.0f, 1.0f, 1.0f);\n \
    }"};

  // Create fragment shader
  GLuint uiFragmentShader;
  if (!GL_LoadShader(uiFragmentShader, GL_FRAGMENT_SHADER,
                     p_cFragmentShaderSource))
    return false;

  // Create program
  if (!GL_LoadShaders(gMainProgram, uiVertexShader, uiFragmentShader))
    return false;

  // Clean up unneeded shaders
  glDeleteShader(uiVertexShader);
  glDeleteShader(uiFragmentShader);

  // Create a Vertex Array Object
  glGenVertexArrays(1, &gVAO);
  glBindVertexArray(gVAO);

  // Create VBO data
  GLfloat fVertexData[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f};

  // Create Vertex Buffer Object
  glGenBuffers(1, &gVBO);
  glBindBuffer(GL_ARRAY_BUFFER, gVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(fVertexData), fVertexData,
               GL_STATIC_DRAW);
  // Specify location of data within buffer
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                        (const GLvoid *)0);
  glEnableVertexAttribArray(0);

  // Specify program to use
  glUseProgram(gMainProgram);

  return true;
};

void GL_Quit() {
  // Release the shader program
  glDeleteProgram(gMainProgram);

  // Delete VBO and VAO
  glDeleteBuffers(1, &gVBO);
  glDeleteVertexArrays(1, &gVAO);
};

void GL_Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Specify VAO to use
  glBindVertexArray(gVAO);

  // Draw the triangle
  glDrawArrays(GL_TRIANGLES, 0, 3);
};

int main(int, char **) {
  init();

  if (GL_Init()) {
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
      GL_Render();

      // Swap the back-buffer and present it
      SDL_GL_SwapWindow(gWindow);
    }
  }
  GL_Quit();

  cleanup();

  return 0;
}
