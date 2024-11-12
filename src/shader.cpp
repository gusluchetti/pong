#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_rwops.h>

GLchar *GL_LoadShaderfile(const char *p_cFileName) {
  // Load directly from the specified file
  SDL_RWops *p_File = SDL_RWFromFile(p_cFileName, "r");
  if (p_File == NULL) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to open shader file: %s\n", p_cFileName);
    exit(1);
  }

  // Allocate a string to store contents
  size_t lFileSize = (size_t)SDL_RWsize(p_File);
  GLchar *p_cFileContents = (GLchar *)malloc(lFileSize + 1);
  SDL_RWread(p_File, p_cFileContents, lFileSize, 1);
  p_cFileContents[lFileSize] = '\0'; // Add terminating character

  return p_cFileContents;
}

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
