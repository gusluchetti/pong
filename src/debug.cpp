#include <GL/glew.h>
#include <SDL2/SDL.h>

static char *gp_cSeverity[] = {"High", "Medium", "Low", "Notification"};
static char *gp_cType[] = {"Error",       "Deprecated",  "Undefined",
                           "Portability", "Performance", "Other"};
static char *gp_cSource[] = {"OpenGL",    "OS",          "GLSL Compiler",
                             "3rd Party", "Application", "Other"};

void DebugCallback(GLenum uiSource, GLenum uiType, GLuint uiID,
                   GLenum uiSeverity, GLsizei iLength, const GLchar *p_cMessage,
                   const void *p_UserParam) {
  // Get the severity
  uint32_t uiSevID = 3;
  switch (uiSeverity) {
  case GL_DEBUG_SEVERITY_HIGH:
    uiSevID = 0;
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    uiSevID = 1;
    break;
  case GL_DEBUG_SEVERITY_LOW:
    uiSevID = 2;
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
  default:
    uiSevID = 3;
    break;
  }
  // Get the type
  uint32_t uiTypeID = 5;
  switch (uiType) {
  case GL_DEBUG_TYPE_ERROR:
    uiTypeID = 0;
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    uiTypeID = 1;
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    uiTypeID = 2;
    break;
  case GL_DEBUG_TYPE_PORTABILITY:
    uiTypeID = 3;
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    uiTypeID = 4;
    break;
  case GL_DEBUG_TYPE_OTHER:
  default:
    uiTypeID = 5;
    break;
  }
  // Get the source
  uint32_t uiSourceID = 5;
  switch (uiSource) {
  case GL_DEBUG_SOURCE_API:
    uiSourceID = 0;
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    uiSourceID = 1;
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    uiSourceID = 2;
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    uiSourceID = 3;
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    uiSourceID = 4;
    break;
  case GL_DEBUG_SOURCE_OTHER:
  default:
    uiSourceID = 5;
    break;
  }

  // Output to the Log
  SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                  "OpenGL Debug: Severity=%s, Type=%s, Source=%s - %s",
                  gp_cSeverity[uiSevID], gp_cType[uiTypeID],
                  gp_cSource[uiSourceID], p_cMessage);
  if (uiSeverity == GL_DEBUG_SEVERITY_HIGH) {
    // This a serious error so we need to shutdown the program
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
  }
};
