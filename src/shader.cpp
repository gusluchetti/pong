#include <GL/glew.h>
#include <SDL2/SDL.h>

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
